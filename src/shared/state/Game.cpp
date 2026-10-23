//
// Created by 33651 on 15/10/2026.
//

#include "Game.h"
#include <mutex>
#include <iostream>
#include "Player.h"

state::Game* state::Game::instance = nullptr;
std::mutex state::Game::mutex;

state::Game::~Game() {
    for (Player* p : players) {
        delete p;
    }
}

state::Game * state::Game::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new Game();
    }
    return instance;
}

const state::Board & state::Game::getBoard() const{
    return board;
}

std::vector<state::Turn> state::Game::getTurns() const{
    return turns;
}

std::vector<state::Player*> state::Game::getPlayers() const{
    return players;
}

void state::Game::destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    delete instance;
    instance = nullptr;
}

state::Game::Game() {
    std::cout << "--- Constructeur Game ---" << std::endl;
    players.push_back(new Player("white"));
    players.push_back(new Player("blue"));
    players.push_back(new Player("red"));
    players.push_back(new Player("green"));
    players.push_back(new Player("yellow"));
    current_player = players.front();
}

