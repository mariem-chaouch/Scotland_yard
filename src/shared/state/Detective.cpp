//
// Created by eya on 10/20/26.
//
#include "Detective.h"

namespace state {

    Detective::Detective(std::string color)
        : Player(color), nb_taxi(11), nb_bus(8), nb_train(4) {}

    int Detective::getNbOfTickets(Ticket ticket)
    {
        switch (ticket)
        {
            case TaxiTicket: return nb_taxi;
            case BusTicket: return nb_bus;
            case TrainTicket: return nb_train;
            default:  return -1;
        }
    }

    void Detective::useTransportTicket(Ticket ticket) {
        if (ticket == TaxiTicket && nb_taxi > 0)        nb_taxi--;
        else if (ticket == BusTicket && nb_bus > 0)     nb_bus--;
        else if (ticket == TrainTicket && nb_train > 0) nb_train--;
    }

}