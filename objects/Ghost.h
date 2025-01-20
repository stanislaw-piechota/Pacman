#ifndef GHOST_H
#define GHOST_H

#include "MovingEntity.h"
#include "../algorithms/Graph.h"
#include "Player.h"

class Ghost : public MovingEntity
{
    Q_OBJECT

public:
    Ghost(const char *imagePath, int initialX = globals::blinkyInitialX, int initialY = globals::blinkyInitialY, int initialWaitingTime = 0);
    ~Ghost();
    virtual void chooseDirection(Graph *graph, const Player *player);
    void extractDirection(const Point &start, const Point &end, Graph *graph, bool panic);
    void setupPosition();
    void setDirection(Directions direction) override;
    void activateCruiseElroy();
    const Point findRunawayCorner(const Point &from);
    QTimer *movementTimer, *panicTimer, *blinkTimer;
    int waitingTime;
    bool panicMode = false;
    bool eaten = false;
    bool blinking = false;
    static int currentGhostPoints;
    void reset();
    void setEeatenMode();
    void resetPanicMode();
    inline void initializeMovement() { movementTimer->start(waitingTime); }
public slots:
    void setPanicMode();
protected slots:
    void nextFrame() override;
protected:
    int initialX, initialY;
    bool inCage = true, isCruiseElroy = false;
};

#endif