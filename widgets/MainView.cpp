#include "MainView.h"
#include <QString>
#include "globals.h"
#include <iostream>

using namespace globals;

MainView::MainView(QWidget *parent) : QWidget(parent)
{
    setFixedSize(windowWidth, windowHeight);
    setStyleSheet("background-color: black");

    board = new Board(this);
    board->move((width() - board->width()) / 2, (height() - board->height()) / 2);
    setupWidgets();
    setupConnections();
    board->readLevel(board->getLevel());
};

MainView::~MainView()
{
    if (board)
        delete board;
    delete pointsLabel;
    delete readyLabel;
    delete levelLabel;
    for (int i = 0; i < numberOfLives; i++)
        delete lives[i];
}

void MainView::setupWidgets()
{
    pointsLabel = new UpdatableLabel("1UP\n ", createFont(), "white", this);
    pointsLabel->move(marginOffset * 2, 10);
    pointsLabel->update(0);
    levelLabel = new UpdatableLabel("LEVEL ", createFont(), "white", this);
    levelLabel->move((boardWidth - levelLabel->widthMM()) / 2, 20);
    levelLabel->update(board->getLevel() + 1);
    readyLabel = new UpdatableLabel("READY?", createFont(10), "yellow", this);
    readyLabel->move((windowWidth - readyLabel->width()) / 2, readyLabelY);
    createLivesLabels();
}

void MainView::setupConnections(){
    connect(board, &Board::addPointsSignal, pointsLabel, static_cast<void (UpdatableLabel::*)(const int &)>(&UpdatableLabel::update));
    connect(board, &Board::lifeLost, this, &MainView::updateLives);
    connect(board, &Board::hideReadyLabel, readyLabel, &QLabel::hide);
    connect(board, &Board::showReadyLabel, readyLabel, &QLabel::show);
    connect(board, &Board::createFruitLabel, this, &MainView::createFruitLabel);
}

void MainView::createLivesLabels()
{
    for (int i = 0; i < numberOfLives; i++)
    {
        lives[i] = new ImageLabel(lifeImagePath, this);
        lives[i]->move(marginOffset + i * lives[i]->width(), marginOffset + boardHeight);
    }
}

void MainView::updateLives(int left)
{
    levelLabel->update(board->getLevel() + 1);
    for (int i = 0; i < left; i++)
        lives[i]->show();
    for (int i = left; i < numberOfLives; i++)
        lives[i]->hide();
    if (!left)
    {
        emit gameLost(board->getPoints());
        board->stopEvents();
    }
}

void MainView::createFruitLabel(EatableEntity *entity)
{
    ImageLabel *label = new ImageLabel(entity, this);
    fruits << label;
    connect(entity, &EatableEntity::spawnFruit, this, &MainView::removeFruit);
    displayFruits();
}

void MainView::removeFruit(EatableEntity *eatable)
{
    ImageLabel *label = fruits[0];
    int i;
    for (i = 0; i < fruits.length() && label->eatable != eatable; i++)
        label = fruits[i];
    label->hide();
    delete label;
    fruits.removeOne(label);
    displayFruits();
}

void MainView::displayFruits()
{
    for (int i = 0; i < fruits.length(); i++)
        fruits[i]->move(marginOffset + boardWidth - i * fruits[i]->width(), marginOffset + boardHeight);
}