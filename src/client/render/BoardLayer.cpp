#include "BoardLayer.h"

#include <SFML/Graphics.hpp>

namespace render {

    BoardLayer::BoardLayer(const state::Board& board)
        : board(board)
    {
    }

    void BoardLayer::init()
    {
        // TODO: Load board texture.
        // TODO: Prepare station and connection graphics.
    }

    void BoardLayer::draw(sf::RenderTarget& target)
    {
        // Temporary background
        sf::RectangleShape background({650.f, 550.f});

        background.setPosition(20.f, 20.f);
        background.setFillColor(sf::Color(235, 235, 235));

        target.draw(background);
    }

}