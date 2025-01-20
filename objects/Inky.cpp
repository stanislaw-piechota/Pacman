#include "Inky.h"
#include <cstdlib>
#include <ctime>

Inky::Inky() : Ghost(globals::inkyTilesetPath, globals::inkyInitialX, globals::inkyInitialY, globals::inkyInitialWaitingTime) {}

void Inky::chooseDirection(Graph *graph, const Player *player)
{
    Ghost::chooseDirection(graph, player);
    if (graph->isTurn(getCell()) && reachedCellMiddle() && !panicMode && !eaten)
    {
        switchPredicateMode(player);
        extractDirection(getCell(), destinationPredicate(graph, player), graph, false);
    }
}

void Inky::switchPredicateMode(const Player *player)
{
    if (heuristic(player->pos(), pos()) <= globals::ghostBarrierDistance)
        mode = rand() % 2;
}

Point Inky::destinationPredicate(Graph *graph, const Player *player)
{
    return mode ? getLastInRow(graph, player) : globals::corners[rand() % 4];
}

Point Inky::getLastInRow(Graph *graph, const Player *player)
{
    Point current = player->getCell();
    while (graph->containsNext(current, player->getCurrentDirection()) && !graph->isTurn(current))
        current = graph->next(current, player->getCurrentDirection());
    return current;
}