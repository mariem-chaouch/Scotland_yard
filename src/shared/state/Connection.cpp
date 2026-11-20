//
// Created by 33651 on 15/10/2026.
//
#include "Connection.h"

#include <iostream>

state::Connection::Connection(Transport mean): mean (mean) {

}

state::Connection::~Connection() = default;

state::Transport state::Connection::getTransport() const{
    return mean;
}
