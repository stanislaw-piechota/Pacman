#ifndef GRAPH_H
#define GRAPH_H

#include "../globals.h"
#include <vector>
#include <cstdlib>

class Graph
{
public:
    ~Graph();
    inline void add(int row, int column) { matrix.push_back(Point(column * globals::cellSize, row * globals::cellSize)); };
    inline void add(const Point point) { matrix.push_back(point); };
    inline bool contains(const Point &point) { return std::find(matrix.begin(), matrix.end(), point) != matrix.end(); };
    inline bool containsNext(const Point &origin, Directions dir) { return contains(next(origin, dir)); };
    inline const Point& random() { return matrix.at(rand() % matrix.size()); }
    inline bool isTurn(const Point &point)
    {
        return (containsNext(point, Directions::Left) ||
                containsNext(point, Directions::Right)) &&
               (containsNext(point, Directions::Up) ||
                containsNext(point, Directions::Down));
    }
    inline Point next(const Point &origin, Directions dir) const
    {
        return Point(mmod(origin.x() + getDeltaX(dir, globals::cellSize), globals::boardWidth),
                     mmod(origin.y() + getDeltaY(dir, globals::cellSize), globals::boardHeight));
    };
    inline Point prev(const Point &origin, Directions dir) const
    {
        return next(origin, globals::oppositeDirections[dir]);
    }
    std::vector<Point> aStar(const Point &start, const Point &end, Directions forbidden, bool panic);

private:
    std::vector<Point> matrix;
    std::vector<Point> restorePath(std::map<Point, Point> &parents, const Point &start, const Point &end);
};

#endif