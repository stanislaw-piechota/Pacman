#ifndef GLOBALS_H
#define GLOBALS_H

#include <map>
#include <QPointF>
#include <QFontDatabase>
#include <QDebug>
#include <ostream>

enum class Directions
{
    Left = 0,
    Up = 1,
    Right = 2,
    Down = 3
};

struct Point
{
public:
    Point() = default;
    Point(int x, int y) : _x(x), _y(y) {};
    Point(const QPointF &p) : _x(p.x()), _y(p.y()) {};
    inline int x() const { return _x; };
    inline int y() const { return _y; };
    double heuristic(const Point &other) const;
    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;
    bool operator<(const Point &other) const;

private:
    int _x, _y;
};
std::ostream &operator<<(std::ostream &s, const Point &p);
inline double heuristic(const Point &a, const Point &b) { return a.heuristic(b); };

int getDeltaX(Directions dir, int step);
int getDeltaY(Directions dir, int step);
Directions reverseDelta(int deltaX, int deltaY);
Directions reverseDelta(const Point &p1, const Point &p2);
int mmod(int a, int b);

namespace globals
{
    const static int step = 5;
    const static int rows = 31, columns = 28, cellSize = 20;
    const static int boardWidth = columns * cellSize, boardHeight = rows * cellSize;
    const static int marginOffset = 50;
    const static int windowWidth = boardWidth + 2 * marginOffset, windowHeight = boardHeight + 2 * marginOffset;
    const static int readyLabelY = marginOffset + 17 * cellSize;
    const int angles[4] = {0, 90, 180, 270};
    const static int smallFontSize = 12;
    const static char emulogicFontPath[] = "assets/emulogic_font.ttf", pacFontPath[] = "assets/PAC-FONT.ttf";
    const static char levelsLayoutPath[] = "assets/levels_description.csv";

    const static int entityAnimationTime = 500;

    const static char pacmanTilesetPath[] = "assets/pacman_tileset.png";
    const static int pacmanTiles = 2, pacmanImageSize = 20, pacmanMovementTime = 35, eyesMovementTime = 15;
    const static int playerInitialRow = 23, playerInitialColumn = 13;
    const static int playerInitialX = playerInitialColumn * cellSize, playerInitialY = playerInitialRow * cellSize;

    const static int ghostTotalTiles = 13, ghostDirectionTiles = 2, ghostImageSize = 30;
    const static int panicTileIndex = 4, blinkTileIndex = 5, eyesTileIndex = 12;
    extern int blinkingInitialTime, panicInitialTime, noBlinkingPanicTime, ghostMovementTime; // level variables
    const static int blinkSwitchTime = 500, oneSecond = 1000;
    const static int actionDelayTime = 3000;

    const static char blinkyTilesetPath[] = "assets/blinky_tileset.png";
    const static int blinkyInitialX = 265, blinkyInitialY = 215, cruiseElroyPoints = 60, cruiseElroyDecrement = 10;
    const static char pinkyTilesetPath[] = "assets/pinky_tileset.png";
    const static int pinkyInitialX = 265, pinkyInitialY = 275, pinkyInitialWaitingTime = 2500;
    const static char inkyTilesetPath[] = "assets/inky_tileset.png";
    const static int inkyInitialX = 225, inkyInitialY = 275, inkyInitialWaitingTime = 7500;
    const static char clydeTilesetPath[] = "assets/clyde_tileset.png";
    const static int clydeInitialX = 305, clydeInitialY = 275, clydeInitialWaitingTime = 10000, ghostBarrierDistance = 250;
    const Point corners[4] = {Point(520, 20), Point(20, 20), Point(20, 580), Point(520, 580)};
    const static Point clydeCorner = corners[1], inkyCorner = corners[3];
    const static Point spawnPoint(260, 220);
    const static char deathGifPath[] = "assets/death_gif/death.gif";
    const static char lifeImagePath[] = "assets/life_image.png";
    const static int numberOfLives = 3;

    const static char boardTilesetPath[] = "assets/board_tileset.png";
    const static char boardLayoutPath[] = "assets/board_layout.csv";
    const static int pointTileIndex = 23, powerupTileIndex = 24, emptyTileIndex = 33,
                     cherryTileIndex = 34, strawberryTileIndex = 35, orangeTileIndex = 36,
                     appleTileIndex = 37, grenadeTileIndex = 38, matchTileIndex = 39,
                     bellTileIndex = 40, keyTileIndex = 41;
    const static int basePoints = 10, powerupPoints = 50, cherryPoints = 100,
                     strawberryPoints = 300, orangePoints = 500, applePoints = 700,
                     grenadePoints = 1000, matchPoints = 2000, bellPoints = 3000, keyPoints = 5000,
                     baseGhostPoints = 200, ghostPointsMultiplier = 2;
    const static int maxFruitSpawnTime = 30000, fruitSpawnDuration = 20000;
    inline std::map<int, int> tileIndexToPoints = {
        {pointTileIndex, basePoints}, {powerupTileIndex, powerupPoints}, {emptyTileIndex, 0},
        {cherryTileIndex, cherryPoints}, {strawberryTileIndex, strawberryPoints}, {orangeTileIndex, orangePoints},
        {appleTileIndex, applePoints}, {grenadeTileIndex, grenadePoints}, {matchTileIndex, matchPoints},
        {bellTileIndex, bellPoints}, {keyTileIndex, keyPoints}
    };
    inline std::map<Directions, Directions> oppositeDirections = {
        {Directions::Left, Directions::Right},
        {Directions::Right, Directions::Left},
        {Directions::Up, Directions::Down},
        {Directions::Down, Directions::Up}};
    inline std::map<char, Directions> charToDirection = {
        {'L', Directions::Left},
        {'R', Directions::Right},
        {'U', Directions::Up},
        {'D', Directions::Down}};
}

QFont createFont(int size = globals::smallFontSize, const char *fontPath = globals::emulogicFontPath);

#endif