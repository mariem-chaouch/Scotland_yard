//
// Created by 33651 on 15/10/2026.
//

#include "Game.h"
#include <iostream>

#include "Detective.h"
#include "MrX.h"


state::Game::~Game() {
    for (Player* p : players) {
        delete p;
    }
}

const state::Board & state::Game::getBoard() const{
    return board;
}

int state::Game::getTurn() const{
    return turn;
}

std::vector<state::Player*> state::Game::getPlayers() const{
    return players;
}

state::Game::Game() {
    std::cout << "--- Constructeur Game ---" << std::endl;
    players.push_back(new MrX("white"));
    players.push_back(new Detective("blue"));
    players.push_back(new Detective("red"));
    players.push_back(new Detective("green"));
    players.push_back(new Detective("yellow"));
    current_player = players.front();
    turn = 0;
}

