#ifndef EAT_ENTITY_H
#define EAT_ENTITY_H
#include "Entity.h"
#include <cstdlib>
#include <QTimer>
#include <iostream>
#include <QList>

class EatableEntity : public Entity
{
    Q_OBJECT

public:
    EatableEntity(const QPixmap &sourceTileset, int index, int points = globals::basePoints, bool isPowerup = false, bool isFruit = false) : Entity(sourceTileset, index)
    {
        this->points = points;
        this->isFruit = isFruit;
        this->isPowerup = isPowerup;
        setZValue(1);

        spawnTime = rand() % (globals::maxFruitSpawnTime - 1);
        spawnTimer = new QTimer(this);
        spawnTimer->setSingleShot(true);
        spawnTimer->start(spawnTime);
        deathTimer = new QTimer(this);
        deathTimer->setSingleShot(true);
        connect(spawnTimer, &QTimer::timeout, this, [&](){
            emit spawnFruit(this);
            deathTimer->start(globals::fruitSpawnDuration);
        });
        connect(deathTimer, &QTimer::timeout, this, [&](){
            emit hideFruit(this);
        });
    }
    ~EatableEntity()
    {
        delete spawnTimer;
    }
    inline void setPos(const Point& p){
        Entity::setPos(p.x(), p.y());
    }
    inline void setPos(int x, int y){
        Entity::setPos(x, y);
    }

    int points;
    bool isPowerup, isFruit, isSpawned = true;
    QTimer *spawnTimer, *deathTimer;
    int spawnTime;
signals:
    void spawnFruit(EatableEntity *eatable);
    void hideFruit(EatableEntity *eatable);

};

#endif