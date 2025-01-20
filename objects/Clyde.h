#ifndef CLYDE_H
#define CLYDE_H

#include "Ghost.h"

class Clyde : public Ghost {
    Q_OBJECT

public:
    Clyde();
    ~Clyde() = default;
    void chooseDirection(Graph *graph, const Player *player) override;
};

#endif