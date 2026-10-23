#include "Move.h"

namespace state {
    Move::Move (int destinationStationId, Ticket ticket, Player* player)
    {
        this->destinationStationId = destinationStationId;
        this->ticket = ticket;
        this->player = player;
    }

    Move::~Move() = default;

    int Move::getDestination ()
    {
        return destinationStationId;
    }
    Player* Move::getPlayer ()
    {
        return player;
    }
    Ticket Move::getTicket ()
    {
        return ticket;
    }

}