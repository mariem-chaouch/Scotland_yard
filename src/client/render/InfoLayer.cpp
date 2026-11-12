#include "InfoLayer.h"

#include <SFML/Graphics.hpp>

namespace render {

    InfoLayer::InfoLayer()
    {
    }

    void InfoLayer::init()
    {
        // TODO: Load fonts and prepare interface.
    }

    void InfoLayer::draw(sf::RenderTarget& target)
    {
        sf::RectangleShape panel({220.f, 500.f});

        panel.setPosition(700.f, 50.f);
        panel.setFillColor(sf::Color(220, 220, 220));
        panel.setOutlineColor(sf::Color::Black);
        panel.setOutlineThickness(2.f);

        target.draw(panel);
    }

}