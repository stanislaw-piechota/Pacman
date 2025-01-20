#include "Player.h"
#include <QLabel>

using namespace globals;

Player::Player() : MovingEntity(pacmanTilesetPath, pacmanTiles, pacmanImageSize)
{
    setPos(playerInitialX, playerInitialY);
    rotationAngle = angles[0];
    setZValue(3);
    
    deathGif = new QMovie(globals::deathGifPath);
    connect(deathGif, &QMovie::frameChanged, this, &Player::updateGif);
    connect(deathGif, &QMovie::finished, this, &Player::emitLifeLost);
}

Player::~Player() {
    delete deathGif;
}

void Player::setDirection(Directions direction)
{
    setRotationAngle(angles[static_cast<int>(direction)]);
    currentDirection = direction;
    movement = true;
    animation = true;
}

void Player::setRotationAngle(int newAngle)
{
    rotateTileset(newAngle - rotationAngle);
    rotationAngle = newAngle;
    updateTile(currentFrame);
}

bool Player::collidesWithEatable(EatableEntity *eatable){
    return collidesWithItem(eatable) && eatable->isSpawned;
}

void Player::startDeathGif()
{
    animation = false;
    deathGif->start();
}

void Player::nextFrame()
{
    if (!animation)
        return;

    currentFrame = (currentFrame + 1) % frameCount;
    updateTile(currentFrame);
}

void Player::updateGif()
{
    setPixmap(deathGif->currentPixmap());
    update();
}

void Player::rotateTileset(int newAngle)
{
    QTransform transform;
    transform.rotate(newAngle);

    for (int i = 0; i < frameCount; i++)
        tileset[i] = tileset[i].transformed(transform, Qt::SmoothTransformation);
}
