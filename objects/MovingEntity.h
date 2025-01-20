#ifndef MOV_ENTITY_H
#define MOV_ENTITY_H

#include "Entity.h"
#include <QTimer>

class MovingEntity : public Entity
{
    Q_OBJECT

public:
    MovingEntity(const char *imagePath, int tiles, int imageSize = globals::cellSize) : Entity(imagePath, tiles, imageSize)
    {
        frameCount = tiles;
        currentFrame = 0;

        animationTimer = new QTimer();
        connect(animationTimer, &QTimer::timeout, this, &MovingEntity::nextFrame);
        animationTimer->start(globals::entityAnimationTime);
    };
    ~MovingEntity(){
        delete animationTimer;
    }

    inline void switchOffAnimation() { animation = false; animationTimer->stop(); };
    inline void turnOnAnimation() { animation = true; animationTimer->start(globals::entityAnimationTime); }
    inline void switchOffMovement() { movement = false; };
    inline bool isMoving() const { return movement; };
    inline Point getCell() const
    {
        return Point((int)(x() + width() / 2) / globals::cellSize * globals::cellSize,
                     (int)(y() + height() / 2) / globals::cellSize * globals::cellSize);
    };
    inline bool reachedCellMiddle()
    {
        Point cell = getCell();
        return ((int)(x() + width() / 2) == cell.x() + globals::cellSize / 2) &&
               ((int)(y() + height() / 2) == cell.y() + globals::cellSize / 2);
    }
    inline void move()
    {
        if (!movement)
            return;

        setPos(x() + getDeltaX(currentDirection, globals::step), y() + getDeltaY(currentDirection, globals::step));
        if (x() + width() <= 0)
            setPos(globals::boardWidth - width(), y());
        if (x() >= globals::boardWidth)
            setPos(0, y());
        if (y() + height() <= 0)
            setPos(x(), globals::boardHeight - height());
        if (y() >= globals::boardHeight)
            setPos(x(), 0);
    }

    inline Directions getCurrentDirection() const { return currentDirection; };
    virtual void setDirection(Directions direction) = 0;
protected slots:
    virtual void nextFrame() = 0;

protected:
    QTimer *animationTimer;
    Directions currentDirection = Directions::Left;
    bool animation = false;
    int frameCount, currentFrame;
    bool movement = false;
};

#endif