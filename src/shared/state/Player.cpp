
#include "Player.h"

namespace state {

    Player::Player(std::string color) : color(color), currentStationId(0) {}

    Player::~Player() {}

    std::string Player::getColor() {
        return color;
    }

    int Player::getCurrentStationId() {
        return currentStationId;
    }

    bool Player::makeMove(int destinationId, Ticket ticket) {
        currentStationId = destinationId;
        useTransportTicket(ticket);
        return true;
    }


    void Player::useTransportTicket(Ticket ticket) {
        (void)ticket;
    }


    void Player::setColor(const std::string& color) {
        this->color = color;
    }

    void Player::setCurrentStationId(int currentStationId) {
        this->currentStationId = currentStationId;
    }

}
