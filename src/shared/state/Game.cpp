//
// Created by 33651 on 15/10/2026.
//

#include "Game.h"
#include <mutex>
#include <iostream>
#include "Player.h"

state::Game* state::Game::instance = nullptr;
std::mutex state::Game::mutex;

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
//TODO : Remplacer les constructeurs Player par MrX et Detective
state::Game::Game() : current_player(players.front()) {
    std::cout << "--- Constructeur Game ---" << std::endl;
    players.push_back(*new Player("white"));
    players.push_back(*new Player("blue"));
    players.push_back(*new Player("red"));
    players.push_back(*new Player("green"));
}

