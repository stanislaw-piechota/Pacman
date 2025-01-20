#ifndef BLINKY_H
#define BLINKY_H

#include "Ghost.h"

class Blinky : public Ghost {
    Q_OBJECT

public:
    Blinky();
    ~Blinky() = default;
    void chooseDirection(Graph *graph, const Player *player) override;
};

#endif