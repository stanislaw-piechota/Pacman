#ifndef PLAYER_H
#define PLAYER_H

#include "MovingEntity.h"
#include "EatableEntity.h"
#include <QMovie>

class Player: public MovingEntity {
    Q_OBJECT
public:
    Player();
    ~Player();

    inline void setBufforDirection(Directions newDirection) { bufforDirection = newDirection; };
    inline Directions getBufforDirection() const { return bufforDirection; };
    void setDirection(Directions direction) override;
    void setRotationAngle(int newAngle);

    bool collidesWithEatable(EatableEntity *eatable);
    void startDeathGif();
signals:
    void lifeLost();
protected slots:
    void nextFrame() override;
    void updateGif();
private:
    QMovie *deathGif;
    Directions bufforDirection = Directions::Left;
    int rotationAngle;
    void rotateTileset(int newAngle);
    inline void emitLifeLost() { emit lifeLost(); }
};

#endif