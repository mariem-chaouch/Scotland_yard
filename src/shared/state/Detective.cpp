//
// Created by eya on 10/20/26.
//
#include "Detective.h"

namespace state {

    Detective::Detective(std::string color)
        : Player(color), nb_taxi(11), nb_bus(8), nb_train(4) {}

    bool Detective::hasTicket(Ticket ticket) {
        switch (ticket) {
            case TaxiTicket:  return nb_taxi > 0;
            case BusTicket:   return nb_bus > 0;
            case TrainTicket: return nb_train > 0;
            default:          return false;   // un détective n'a pas de Black Ticket
        }
    }

    void Detective::useTransportTicket(Ticket ticket) {
        if (ticket == TaxiTicket && nb_taxi > 0)        nb_taxi--;
        else if (ticket == BusTicket && nb_bus > 0)     nb_bus--;
        else if (ticket == TrainTicket && nb_train > 0) nb_train--;
    }

}