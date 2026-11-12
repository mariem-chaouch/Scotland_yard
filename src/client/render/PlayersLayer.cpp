#include "PlayersLayer.h"

#include <SFML/Graphics.hpp>

namespace render {

    PlayersLayer::PlayersLayer()
    {
    }

    void PlayersLayer::init()
    {
        // TODO: Load player textures.
    }

    void PlayersLayer::draw(sf::RenderTarget& target)
    {
        sf::CircleShape detective(10.f);
        detective.setFillColor(sf::Color::Blue);
        detective.setPosition(150.f, 150.f);

        sf::CircleShape mrX(10.f);
        mrX.setFillColor(sf::Color::Red);
        mrX.setPosition(450.f, 350.f);

        target.draw(detective);
        target.draw(mrX);
    }

}