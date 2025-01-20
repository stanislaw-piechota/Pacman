#include "Clyde.h"

Clyde::Clyde() : Ghost(globals::clydeTilesetPath, globals::clydeInitialX, globals::clydeInitialY, globals::clydeInitialWaitingTime) {}

void Clyde::chooseDirection(Graph *graph, const Player *player)
{
    Ghost::chooseDirection(graph, player);
    if (graph->isTurn(getCell()) && reachedCellMiddle() && !panicMode && !eaten)
        extractDirection(getCell(),
                         heuristic(player->pos(), pos()) > globals::ghostBarrierDistance ? player->getCell() : globals::clydeCorner,
                         graph, false);
}