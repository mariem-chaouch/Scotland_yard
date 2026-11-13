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

state::GameState::GameState() {
    std::cout << "--- Constructeur Game ---" << std::endl;
    players.push_back(new MrX("white"));
    players.push_back(new Detective("blue"));
    players.push_back(new Detective("red"));
    players.push_back(new Detective("green"));
    players.push_back(new Detective("yellow"));
    current_player = players.front();
    turn = 0;
}

state::Player*state::GameState::getCurrentPlayer() const {
    return current_player;
}

