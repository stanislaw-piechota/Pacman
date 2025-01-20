#include "Ghost.h"
#include <iostream>

using namespace globals;
Ghost::Ghost(const char *imagePath, int initialX, int initialY, int initialWaitingTime) : MovingEntity(imagePath, ghostTotalTiles, ghostImageSize)
{
    this->initialX = initialX;
    this->initialY = initialY;
    this->waitingTime = initialWaitingTime;
    setPos(initialX, initialY);
    setZValue(2);
    movementTimer = new QTimer(this);
    panicTimer = new QTimer(this);
    panicTimer->setSingleShot(this);
    blinkTimer = new QTimer(this);
    blinkTimer->setSingleShot(this);
    connect(panicTimer, &QTimer::timeout, this, &Ghost::resetPanicMode);
    connect(blinkTimer, &QTimer::timeout, [&]()
            { if (panicMode) blinking = true; });
};

Ghost::~Ghost()
{
    delete movementTimer;
    delete panicTimer;
    delete blinkTimer;
}

int Ghost::currentGhostPoints = baseGhostPoints;

void Ghost::chooseDirection(Graph *graph, const Player *player)
{
    if (inCage)
        return setupPosition();
    if (eaten && getCell() == spawnPoint)
    {
        eaten = false;
        updateTile(0);
        animationTimer->start(entityAnimationTime);
        movementTimer->setInterval(ghostMovementTime);
    }
    if (eaten && reachedCellMiddle() && graph->isTurn(getCell()))
        return extractDirection(getCell(), spawnPoint, graph, false);
    if (panicMode && reachedCellMiddle() && graph->isTurn(getCell()))
        return extractDirection(getCell(), findRunawayCorner(player->getCell()), graph, true);
}

void Ghost::activateCruiseElroy()
{
    if (!isCruiseElroy)
        return;

    movementTimer->setInterval(ghostMovementTime - cruiseElroyDecrement);
}

const Point Ghost::findRunawayCorner(const Point &from)
{
    double maxDist = 0, newDist = 0;
    Point maxCorner;

    for (const Point &corner : corners)
    {
        if ((newDist = heuristic(from, corner)) > maxDist)
        {
            maxDist = newDist;
            maxCorner = corner;
        }
    }
    return maxCorner;
}

void Ghost::extractDirection(const Point &start, const Point &end, Graph *graph, bool panic)
{
    std::vector<Point> path = graph->aStar(start, end, oppositeDirections[currentDirection], panic);
    if (path.empty())
        return setDirection(oppositeDirections[currentDirection]);
    if (path.size() == 1) // Player and Ghost are in the same cell but are not colliding yet
        return;
    setDirection(reverseDelta(path[0], path[1]));
}

void Ghost::nextFrame()
{
    if (!animation)
        return;

    currentFrame = (currentFrame + 1) % ghostDirectionTiles;
    int index = (int)currentDirection;
    if (blinking)
        index = tileIndex == panicTileIndex * ghostDirectionTiles ? blinkTileIndex : panicTileIndex;
    else if (panicMode)
        index = panicTileIndex;
    updateTile(index * ghostDirectionTiles + currentFrame);
}

void Ghost::setDirection(Directions direction)
{
    currentDirection = direction;
    movement = true;
    animation = true;
}

void Ghost::setupPosition()
{
    movementTimer->setInterval(ghostMovementTime);
    int deltaX = blinkyInitialX - x(), deltaY = blinkyInitialY - y();
    if (!deltaX && !deltaY)
    {
        inCage = false;
        setDirection(Directions::Left);
        return;
    }
    setDirection(reverseDelta(deltaX, deltaY));
}

void Ghost::setPanicMode()
{
    if (inCage || eaten)
        return;

    blinking = false;
    panicMode = true;
    blinkTimer->stop();
    panicTimer->stop();
    panicTimer->start(panicInitialTime);
    blinkTimer->start(noBlinkingPanicTime);
    movementTimer->setInterval(ghostMovementTime * 4 / 3);
    updateTile(panicTileIndex * ghostDirectionTiles);
}

void Ghost::resetPanicMode()
{
    if (!panicMode)
        return;

    eaten = false;
    blinking = false;
    panicMode = false;
    updateTile(0);
    panicTimer->stop();
    blinkTimer->stop();
    movementTimer->setInterval(ghostMovementTime);
}

void Ghost::setEeatenMode()
{
    eaten = true;
    panicMode = false;
    blinking = false;
    panicTimer->stop();
    blinkTimer->stop();
    updateTile(eyesTileIndex);
    animationTimer->stop();
    movementTimer->setInterval(eyesMovementTime);
}

void Ghost::reset()
{
    inCage = true;
    panicMode = false;
    eaten = false;
    blinking = false;
    movementTimer->stop();
    updateTile(0);
    setPos(initialX, initialY);
}