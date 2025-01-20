#include "Pinky.h"

Pinky::Pinky() : Ghost(globals::pinkyTilesetPath, globals::pinkyInitialX, globals::pinkyInitialY, globals::pinkyInitialWaitingTime) {}

void Pinky::chooseDirection(Graph *graph, const Player *player)
{
    Ghost::chooseDirection(graph, player);
    if (graph->isTurn(getCell()) && reachedCellMiddle() && !panicMode && !eaten)
        extractDirection(getCell(), getLastInRow(graph, player), graph, false);
}

Point Pinky::getLastInRow(Graph *graph, const Player *player)
{

    Point current = player->getCell();
    while (graph->containsNext(current, player->getCurrentDirection()) && !graph->isTurn(current))
        current = graph->next(current, player->getCurrentDirection());
    return current;
}
