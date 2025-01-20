#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

using namespace std;
using namespace globals;

Board::Board(QWidget *parent) : QGraphicsView(parent)
{
    tileset = QPixmap(boardTilesetPath);
    scene = new QGraphicsScene(parent);
    setupSettings();

    createObjects();
    createBoardFromCsv();
    setupConnections();
}

Board::~Board()
{
    delete graph;
    delete player;

    qDeleteAll(eatables);
    eatables.clear();

    for (const auto ghost : ghosts)
        delete ghost;
    ghosts.clear();

    delete scene;
}

void Board::createObjects()
{
    playerTimer = new QTimer(this);
    actionDelayTimer = new QTimer(this);
    actionDelayTimer->setSingleShot(true);

    graph = new Graph;

    player = new Player;
    scene->addItem(player);

    ghosts.push_back(new Blinky);
    ghosts.push_back(new Pinky);
    ghosts.push_back(new Inky);
    ghosts.push_back(new Clyde);
    for (const auto &ghost : ghosts)
        scene->addItem(ghost);
}

void Board::startMovement()
{
    for (auto &ghost : ghosts)
        ghost->initializeMovement();
    playerTimer->start(pacmanMovementTime);
    emit hideReadyLabel();
}

void Board::setPlayerBufforDirection(Directions dir)
{
    player->setBufforDirection(dir);
}

void Board::setupSettings()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(0, 0, boardWidth, boardHeight);
    setFixedSize(boardWidth, boardHeight);
    setStyleSheet("background-color: black");
    setScene(scene);
}

void Board::setupConnections()
{
    for (const auto &ghost : ghosts)
    {
        connect(ghost->movementTimer, &QTimer::timeout, ghost, [&]()
                {
            ghost->chooseDirection(graph, player);
            ghost->move(); });
        connect(this, &Board::setPanicMode, ghost, &Ghost::setPanicMode);
    }

    connect(playerTimer, &QTimer::timeout, this, &Board::performPlayerMove);
    connect(playerTimer, &QTimer::timeout, this, &Board::checkCollisions);
    connect(player, &Player::lifeLost, this, &Board::hideLife);
    connect(actionDelayTimer, &QTimer::timeout, this, &Board::startMovement);
}

void Board::hideLife()
{
    reset();
    emit lifeLost(--lives);
}

void Board::createBoardFromCsv()
{
    ifstream file(boardLayoutPath);
    checkLayoutFileExists(file);

    string line;
    int row = 0, column = 0;
    while (getline(file, line))
    {
        column = 0;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ','))
            createBoardTile(row, column++, item);
        row++;
    }
};

void Board::readLevel(int level)
{
    ifstream file(levelsLayoutPath);
    string line, item;
    for (int i = 0; i <= level && getline(file, line); i++)
        ;
    stringstream ss(line);

    getline(ss, item, ',');
    ghostMovementTime = stoi(item);
    getline(ss, item, ',');
    panicInitialTime = stoi(item);
    if ((noBlinkingPanicTime = panicInitialTime - blinkingInitialTime) < 0)
    {
        blinkingInitialTime = panicInitialTime;
        noBlinkingPanicTime = 1;
    }

    for (int tile = cherryTileIndex; tile <= keyTileIndex; tile++)
    {
        getline(ss, item, ',');
        for (int i = 0; i < stoi(item); i++)
            createEatableTile(tile);
    }
}

void Board::createEatableTile(int tile)
{
    EatableEntity *eatable = new EatableEntity(tileset, tile, tileIndexToPoints[tile], false, true);
    scene->addItem(eatable);
    eatable->hide();
    eatable->isSpawned = false;
    eatables << eatable;

    connect(eatable, &EatableEntity::spawnFruit, this, &Board::spawnFruit);
    connect(eatable, &EatableEntity::hideFruit, this, &Board::removeEatable);
    emit createFruitLabel(eatable);
}

void Board::spawnFruit(EatableEntity *eatable)
{
    eatable->setPos(graph->random());
    eatable->show();
    eatable->isSpawned = true;
}

void Board::removeEatable(EatableEntity *eatable)
{
    scene->removeItem(eatable);
    delete eatable;
    eatables.removeOne(eatable);
}

void Board::createBoardTile(int row, int column, string index)
{
    int tileIndex = stoi(index);
    if (!tileIndex--)
        return;
    sortTile(row, column, tileIndex, createBoardTile(row, column, tileIndex));
}

void Board::sortTile(int row, int column, int index, EatableEntity *item)
{
    if (tileIndexToPoints.find(index) == tileIndexToPoints.end())
        return;

    graph->add(row, column);
    if (index == emptyTileIndex)
        return;
    else if (index == powerupTileIndex)
        item->isPowerup = true;

    item->points = tileIndexToPoints.at(index);
    eatables << item;
}

EatableEntity *Board::createBoardTile(int row, int column, int index)
{
    EatableEntity *newTile = new EatableEntity(tileset, index);
    newTile->setPos(column * cellSize, row * cellSize);
    scene->addItem(newTile);
    return newTile;
}

void Board::checkLayoutFileExists(const ifstream &file)
{
    if (!file.is_open())
    {
        cerr << "Board layout template not found" << endl;
        exit(1);
    }
}

void Board::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        player->setBufforDirection(Directions::Left);
        break;
    case Qt::Key_Right:
        player->setBufforDirection(Directions::Right);
        break;
    case Qt::Key_Up:
        player->setBufforDirection(Directions::Up);
        break;
    case Qt::Key_Down:
        player->setBufforDirection(Directions::Down);
        break;
    case Qt::Key_Escape:
        emit lifeLost(0);
        break;
    }
}

void Board::switchLevels()
{
    if (lives < numberOfLives)
        lives++;
    readLevel(++level);
    emit lifeLost(lives);
    stopGame(false);
    createBoardFromCsv();
    reset();
}

void Board::checkCollisions()
{
    checkEatablesCollisions();

    Ghost *collidedGhost = checkGhostsCollisions();
    if (collidedGhost)
    {
        if (collidedGhost->panicMode)
        {
            emit addPointsSignal(points += Ghost::currentGhostPoints);
            Ghost::currentGhostPoints *= globals::ghostPointsMultiplier;
            collidedGhost->setEeatenMode();
        }
        else
            stopGame();
    }
}

void Board::stopGame(bool death)
{
    for (const auto &ghost : ghosts)
    {
        ghost->reset();
        scene->removeItem(ghost);
        ghost->switchOffAnimation();
    }
    playerTimer->stop();
    if (death)
        player->startDeathGif();
}

void Board::checkEatablesCollisions()
{
    for (EatableEntity *eatable : eatables)
        if (player->collidesWithEatable(eatable))
            destroyEatable(eatable);
}

void Board::destroyEatable(EatableEntity *eatable)
{
    if (eatable->isPowerup)
    {
        Ghost::currentGhostPoints = baseGhostPoints;
        emit setPanicMode();
    }

    addPoints(eatable->points);
    removeEatable(eatable);

    if (eatables.empty())
        switchLevels();
}

void Board::addPoints(int points)
{
    emit addPointsSignal(this->points += points);

    if (++pointStreak == cruiseElroyPoints)
        ghosts[0]->activateCruiseElroy();
}

Ghost *Board::checkGhostsCollisions()
{
    for (const auto &ghost : ghosts)
        if (player->collidesWithItem(ghost) && !ghost->eaten)
            return ghost;
    return nullptr;
}

void Board::performPlayerMove()
{
    if (graph->contains(player->pos()) && graph->containsNext(player->pos(), player->getBufforDirection()))
        player->setDirection(player->getBufforDirection());
    if (graph->contains(player->pos()) && !graph->containsNext(player->pos(), player->getCurrentDirection()))
        player->switchOffMovement();
    player->move();
}

void Board::reset()
{
    for (const auto &ghost : ghosts)
    {
        ghost->reset();
        scene->addItem(ghost);
        ghost->turnOnAnimation();
    }
    pointStreak = 0;
    player->setPos(playerInitialX, playerInitialY);
    player->setDirection(Directions::Left);
    player->setBufforDirection(Directions::Left);
    actionDelayTimer->start(actionDelayTime);
    emit showReadyLabel();
}