#ifndef INKY_H
#define INKY_H

#include "Ghost.h"

class Inky : public Ghost {
    Q_OBJECT

public:
    Inky();
    ~Inky() = default;
    void chooseDirection(Graph *graph, const Player *player) override;
private:
    int mode = 0, predicate = 0;
    void switchPredicateMode(const Player *player);
    Point destinationPredicate(Graph *graph, const Player *player);
    Point getLastInRow(Graph* graph, const Player *player);
};

#endif