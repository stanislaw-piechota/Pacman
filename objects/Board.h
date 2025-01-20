#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QPixmap>
#include <QList>

#include <QTimer>
#include <vector>
#include "globals.h"
#include "Player.h"
#include "EatableEntity.h"
#include "Ghost.h"
#include "../algorithms/Graph.h"

class Board : public QGraphicsView
{
    Q_OBJECT

public:
    Board(QWidget *parent = nullptr);
    ~Board();
    double panicTimeCount = 0;
    QTimer *actionDelayTimer;
    inline void stopEvents() { this->destroy(); }
    inline int getPoints() const { return points; }
    inline int getLevel() const { return level; }
    inline int getLives() const { return lives; }
    void stopGame(bool death=true);
    void readLevel(int i);
    void setPlayerBufforDirection(Directions dir);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void hideLife();
    void checkCollisions();
    void spawnFruit(EatableEntity *eatable);
    void removeEatable(EatableEntity *eatable);
signals:
    void addPointsSignal(int points);
    void setPanicMode();
    void setEatenMode(Ghost *ghost);
    void lifeLost(int left);
    void hideReadyLabel();
    void showReadyLabel();
    void createFruitLabel(EatableEntity *entity);
private:
    QGraphicsScene *scene;
    QTimer *playerTimer;
    Graph *graph;
    int points = 0, lives = globals::numberOfLives, pointStreak = 0, level = 0;
    void createObjects();
    void setupSettings();
    void setupConnections();
    void startMovement();
    void checkLayoutFileExists(const std::ifstream &file);
    void switchLevels();

    QPixmap tileset;
    QList<EatableEntity *> eatables;
    void createBoardFromCsv();
    void createBoardTile(int row, int column, std::string index);
    void createEatableTile(int tile);
    void sortTile(int row, int column, int index, EatableEntity *item);
    EatableEntity *createBoardTile(int row, int column, int index);
    void destroyEatable(EatableEntity *eatable);
    void addPoints(int points);

    Player *player;
    std::vector<Ghost *> ghosts;
    void performPlayerMove();
    Ghost *checkGhostsCollisions();
    void checkEatablesCollisions();
    void reset();
};

#endif