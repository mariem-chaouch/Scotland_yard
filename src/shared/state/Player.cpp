
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
        useTransportTicket(ticket);          // appelle la version de MrX ou de Detective
        currentStationId = destinationId;
        return true;
    }

    // Version de base : ne fait rien. MrX et Detective la redéfinissent.
    void Player::useTransportTicket(Ticket ticket) {
        (void)ticket;
    }

    // Setters générés par dia2code
    void Player::setColor(const std::string& color) {
        this->color = color;
    }

    void Player::setCurrentStationId(int currentStationId) {
        this->currentStationId = currentStationId;
    }

}
