#include "globals.h"
#include <cmath>

int globals::blinkingInitialTime = 3000;
int globals::panicInitialTime = 10000;
int globals::noBlinkingPanicTime = globals::panicInitialTime - globals::blinkingInitialTime;
int globals::ghostMovementTime = 60;

int getDeltaX(Directions dir, int step)
{
    switch (dir)
    {
    case Directions::Left:
        return -step;
        break;
    case Directions::Right:
        return step;
        break;
    case Directions::Up:
        return 0;
        break;
    case Directions::Down:
        return 0;
        break;
    }
    return 0;
}

int getDeltaY(Directions dir, int step)
{
    switch (dir)
    {
    case Directions::Left:
        return 0;
        break;
    case Directions::Right:
        return 0;
        break;
    case Directions::Up:
        return -step;
        break;
    case Directions::Down:
        return step;
        break;
    }
    return 0;
}

Directions reverseDelta(int deltaX, int deltaY)
{
    return deltaX ? (deltaX > 0 ? Directions::Right : Directions::Left) : (deltaY > 0 ? Directions::Down : Directions::Up);
}

Directions reverseDelta(const Point& p1, const Point& p2)
{
    return reverseDelta(p2.x() - p1.x(), p2.y() - p1.y());
}

int mmod(int a, int b)
{
    int result = a % b;
    return result >= 0 ? result : result + b;
}

double Point::heuristic(const Point &other) const { return sqrt(pow(other.x() - _x, 2) + pow(other.y() - _y, 2)); }
bool Point::operator==(const Point &other) const { return (_x == other.x()) && (_y == other.y()); }
bool Point::operator!=(const Point &other) const { return !(*this == other); }
bool Point::operator<(const Point &other) const { return (_x < other.x()) || (_x == other.x() && _y < other.y()); }
std::ostream &operator<<(std::ostream &s, const Point &p)
{
    s << p.x() << ", " << p.y();
    return s;
};

QFont createFont(int size, const char* fontPath)
{
    int fontId = QFontDatabase::addApplicationFont(QString::fromStdString(fontPath));
    if (fontId == -1)
    {
        qDebug() << "Failed to load custom font";
        exit(0);
    }
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    return QFont(fontFamily, size);
}