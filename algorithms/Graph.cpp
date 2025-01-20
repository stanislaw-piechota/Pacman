#include "Graph.h"
#include <queue>
#include <map>
#include "../objects/Ghost.h"
using namespace std;
using namespace globals;

Graph::~Graph() = default;

vector<Point> Graph::aStar(const Point &start, const Point &end, Directions forbidden, bool panic)
{
    if (start == end)
        return {};

    priority_queue<pair<double, Point>, vector<pair<double, Point>>, greater<pair<double, Point>>> open;
    vector<Point> closed;
    map<Point, int> gCost;
    map<Point, double> fCost;
    map<Point, Point> parents;

    gCost[start] = 0;
    fCost[start] = heuristic(start, end);
    open.emplace(fCost[start], start);
    if (containsNext(start, forbidden) && !panic)
        closed.push_back(next(start, forbidden));

    while (!open.empty())
    {
        Point current = open.top().second;
        open.pop();
        closed.push_back(current);

        if (current == end)
            return restorePath(parents, start, end);

        Point neighbour;
        for (int i = 0; i <= (int)Directions::Down; i++)
        {
            neighbour = next(current, static_cast<Directions>(i));
            if (!contains(neighbour) || (find(closed.begin(), closed.end(), neighbour) != closed.end()))
                continue;

            int tentativeG = gCost[current] + cellSize;
            bool neighbourNotFound = gCost.find(neighbour) == gCost.end();
            if (neighbourNotFound || (tentativeG < gCost[neighbour]))
            {
                gCost[neighbour] = tentativeG;
                fCost[neighbour] = tentativeG + heuristic(neighbour, end);
                parents[neighbour] = current;
                if (neighbourNotFound)
                    open.emplace(fCost[neighbour], neighbour);
            }
        }
    }
    return {};
}

std::vector<Point> Graph::restorePath(std::map<Point, Point> &parents, const Point &start, const Point &end)
{
    vector<Point> result;
    Point current = end;
    result.push_back(current);
    while ((current = parents[current]) != start)
        result.push_back(current);
    result.push_back(current);
    reverse(result.begin(), result.end());
    return result;
}