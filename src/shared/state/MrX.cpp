//
// Created by eya on 10/20/26.
//
#include "MrX.h"

namespace state {

    MrX::MrX(std::string color)
        : Player(color), nb_black_tickets(5), nb_double_tickets(2), last_known_station_id(0) {}

    void MrX::use_double_ticket() {
        if (nb_double_tickets > 0) nb_double_tickets--;
    }

    bool MrX::hasBlackTicket() {
        return nb_black_tickets > 0;
    }

    bool MrX::hasDoubleTicket() {
        return nb_double_tickets > 0;
    }

    // Mister X prend taxi/bus/métro dans la pioche commune : seul le Black Ticket se décompte
    void MrX::useTransportTicket(Ticket ticket) {
        if (ticket == BlackTicket && nb_black_tickets > 0) nb_black_tickets--;
    }

}