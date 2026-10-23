#include "Turn.h"

namespace state {

    int Turn::turn_count = 0;

    Turn::Turn(): number(turn_count++)
    {
    }

    Turn::~Turn() = default;

    std::vector<Move> Turn::getMoves()
    {
        return moves;
    }

    int Turn::getNumber()
    {
        return number;
    }

}