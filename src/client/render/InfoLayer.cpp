#include "InfoLayer.h"

#include <SFML/Graphics.hpp>

namespace render {

    InfoLayer::InfoLayer()
    {
    }

    void InfoLayer::init()
    {
        // Load emptyMove texture
        textures.push_back(std::make_unique<sf::Texture>());

        if (!textures.back()->loadFromFile(RESOURCE_DIR "/emptyMove.png"))
            throw std::runtime_error("Failed to load emptyMove.png");

        // Load busMove texture
        textures.push_back(std::make_unique<sf::Texture>());

        if (!textures.back()->loadFromFile(RESOURCE_DIR "/busMove.png"))
            throw std::runtime_error("Failed to load busMove.png");

        // Create 24 emptyMove sprites
        for (int i = 0; i < 24; ++i)
        {
            sprites.emplace_back(*textures[0]);
        }
    }

    void InfoLayer::draw(sf::RenderTarget& target)
    {
        const sf::Vector2u windowSize = target.getSize();

        const float panelHeight = 70.f;
        const float panelWidth = static_cast<float>(windowSize.x);

        sf::RectangleShape panel({panelWidth, panelHeight});

        panel.setFillColor(sf::Color(0, 0, 0));
        panel.setOutlineColor(sf::Color::Black);
        panel.setOutlineThickness(2.f);

        panel.setPosition(
            (windowSize.x - panelWidth) / 2.f,
            windowSize.y - panelHeight
        );
        target.draw(panel);

        // Position the 24 ticket sprites
        const float spriteSize = 45.f;
        const float spriteSpacing = 15.f;

        for (int i = 0; i < sprites.size(); ++i)
        {
            const sf::Vector2u textureSize = textures[0]->getSize();

            const float scale = spriteSize / std::max(textureSize.x, textureSize.y);

            sprites[i].setScale(scale, scale);

            sprites[i].setPosition( windowSize.x /2 + (i - 12) * (spriteSize + spriteSpacing),
                windowSize.y - panelHeight +
                    (panelHeight - textureSize.y * scale) / 2.f
            );

            target.draw(sprites[i]);
        }
    }
}