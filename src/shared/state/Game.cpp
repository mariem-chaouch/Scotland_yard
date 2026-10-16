//
// Created by 33651 on 15/10/2026.
//

#include "Game.h"
#include <mutex>
#include "Player.h"

// state::Game* state::Game::instance = nullptr;
// std::mutex state::Game::mutex;

state::Game * state::Game::getInstance() {
    // std::lock_guard<std::mutex> lock(mutex);
    // if (instance == nullptr) {
    //     instance = new Game();
    // }
    // return instance;
}

state::Board & state::Game::getBoard() {
    return board;
}

std::vector<state::Turn> state::Game::getTurns() {
    return turns;
}

std::vector<state::Player> state::Game::getPlayers() {
    return players;
}

state::Game::Game() : current_player(players.front()) {
    players.push_back(*new Player("white"));
    players.push_back(*new Player("blue"));
    players.push_back(*new Player("red"));
    players.push_back(*new Player("green"));
}

