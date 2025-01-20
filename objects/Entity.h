#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <vector>
#include "globals.h"

class Entity : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Entity() = default;
    Entity(const char *imagePath, int tiles, int imageSize = globals::cellSize) : QGraphicsPixmapItem(), imageSize(imageSize)
    {
        loadTileset(imagePath, tiles);
        updateTile(0);
    }
    Entity(const QPixmap &sourceTileset, int index) : QGraphicsPixmapItem(), imageSize(sourceTileset.height())
    {
        loadTileset(sourceTileset, index);
        updateTile(0);
    }
    ~Entity() = default;
    inline qreal width() const { return boundingRect().width(); };
    inline qreal height() const { return boundingRect().height(); };

protected:
    void loadTileset(const char *imagePath, int tiles)
    {
        if (!tileset.empty())
            tileset.erase(tileset.begin(), tileset.end());

        QPixmap image = QPixmap(imagePath);
        for (int i = 0; i < tiles; i++)
            tileset.push_back(image.copy(i * imageSize, 0, imageSize, imageSize));
    }
    inline void loadTileset(const QPixmap &source, int index)
    {
        if (!tileset.empty())
            tileset.erase(tileset.begin(), tileset.end());

        tileset.push_back(source.copy(index * imageSize, 0, imageSize, imageSize));
        updateTile(0);
    }
    inline void updateTile(int index) { setPixmap(tileset.at(index)); tileIndex = index; };
    std::vector<QPixmap> tileset;
    int imageSize, tileIndex = 0;
};

#endif