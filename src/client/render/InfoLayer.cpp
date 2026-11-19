#include "InfoLayer.h"

#include <SFML/Graphics.hpp>

namespace render {

    InfoLayer::InfoLayer()
    {
    }

    static sf::Font font;

    void InfoLayer::init()
    {
        // Load emptyMove texture
        textures.push_back(std::make_unique<sf::Texture>());

        // Load ticket textures
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/emptyMove.png"))
            throw std::runtime_error("Failed to load emptyMove.png");

        textures.push_back(std::make_unique<sf::Texture>());

        if (!textures.back()->loadFromFile(RESOURCE_DIR "/taxiMove.png"))
            throw std::runtime_error("Failed to load busMove.png");

        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/busMove.png"))
            throw std::runtime_error("Failed to load busMove.png");

        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/trainMove.png"))
            throw std::runtime_error("Failed to load busMove.png");

        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/blackMove.png"))
            throw std::runtime_error("Failed to load busMove.png");



        // Create 24 emptyMove sprites
        for (int i = 0; i < 24; ++i)
        {
            sprites.emplace_back(*textures[0]);
        }

        //Ticket counters
        sprites.emplace_back(*textures[1]);
        sprites.emplace_back(*textures[2]);
        sprites.emplace_back(*textures[3]);
        sprites.emplace_back(*textures[4]);

        if (!font.loadFromFile(RESOURCE_DIR "/arial.ttf"))
            throw std::runtime_error("Failed to load font");

        for (int i = 0; i < 4; ++i)
        {
            texts.emplace_back("0", font, 22);
            texts.back().setFillColor(sf::Color::White);
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

        for (int i = 0; i < 24; ++i)
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

        const float startX = 30.f;
        const float startY = 30.f;
        const float spacing = 55.f;


        for (std::size_t i = 0; i < 4; ++i)
        {
            const sf::Vector2u textureSize = textures[i + 1]->getSize();

            const float scale = spriteSize / std::max(textureSize.x, textureSize.y);

            sprites[24 + i].setScale(scale, scale);

            sprites[24 + i].setPosition(
                startX,
                startY + i * spacing
            );
            texts[i].setPosition(
                startX + 55.f,
                startY + 10 + i * spacing
            );
            target.draw(sprites[24 + i]);
            target.draw(texts[i]);
        }
    }

    /*void InfoLayer::hideSelection()
    {
        showSelection = false;
    }*/
}