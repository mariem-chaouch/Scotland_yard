//
// Created by 33651 on 15/10/2026.
//

#include "GameState.h"
#include <iostream>

#include "Detective.h"
#include "MrX.h"


state::GameState::~GameState() {
    for (Player* p : players) {
        delete p;
    }
}

const state::Board & state::GameState::getBoard() const{
    return board;
}

int state::GameState::getTurn() const{
    return turn;
}

std::vector<state::Player*> state::GameState::getPlayers() const{
    return players;
}
state::Player* state::GameState::getCurrentPlayer() const{
    return current_player;
}

state::GameState::GameState() {
    std::cout << "--- Constructeur Game ---" << std::endl;
    players.push_back(new MrX("white"));
    players.push_back(new Detective("blue"));
    players.push_back(new Detective("red"));
    players.push_back(new Detective("green"));
    players.push_back(new Detective("yellow"));
    // Positions initiales pour les tests
    players[0]->setCurrentStationId(5); // Mister X
    players[1]->setCurrentStationId(1); // Bleu
    players[2]->setCurrentStationId(2); // Rouge
    players[3]->setCurrentStationId(3); // Vert
    players[4]->setCurrentStationId(4); // Jaune

    current_player = players.front();
    turn = 0;
}

void state::GameState::setStationTaken(int stationId, bool taken) {
    board.setStationTaken(stationId, taken);
}
void state::GameState::setTurn(int turn) {
    this->turn = turn;
}

void state::GameState::setCurrentPlayer(Player* player) {
    current_player = player;
}