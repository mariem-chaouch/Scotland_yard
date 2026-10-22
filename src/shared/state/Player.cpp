//
// Created by eya on 10/20/26.
//
#include "Player.h"

namespace state {

    Player::Player(std::string color) : color(color), current_station_id(0) {}

    Player::~Player() {}

    std::string Player::getColor() {
        return color;
    }

    int Player::getCurrentStationId() {
        return current_station_id;
    }

    // Applique un déplacement.
    bool Player::makeMove(int destinationId, Ticket ticket) {
        useTransportTicket(ticket);       
        current_station_id = destinationId;
        return true;
    }

    // Version de base : ne fait rien. MrX et Detective la redéfinissent.
    void Player::useTransportTicket(Ticket ticket) {
        (void)ticket;
    }

    // Getters / setters générés par dia2code
    void Player::setColor(const std::string& color) {
        this->color = color;
    }

    int Player::getCurrent_station_id() const {
        return current_station_id;
    }

    void Player::setCurrent_station_id(int current_station_id) {
        this->current_station_id = current_station_id;
    }

}