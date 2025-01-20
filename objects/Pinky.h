#ifndef PINKY_H
#define PINKY_H

#include "Ghost.h"

class Pinky : public Ghost {
    Q_OBJECT

public:
    Pinky();
    ~Pinky() = default;
    void chooseDirection(Graph *graph, const Player *player) override;
private:
    Point getLastInRow(Graph* graph, const Player *player);
};

#endif