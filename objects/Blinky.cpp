#include "Blinky.h"
#include <vector>

Blinky::Blinky() : Ghost(globals::blinkyTilesetPath) {
    isCruiseElroy = true;
}

void Blinky::chooseDirection(Graph *graph, const Player *player)
{
    Ghost::chooseDirection(graph, player);
    if (graph->isTurn(getCell()) && reachedCellMiddle() && !panicMode && !eaten)
        extractDirection(getCell(), player->getCell(), graph, false);
}
