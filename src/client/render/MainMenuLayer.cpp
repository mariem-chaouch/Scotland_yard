#include "MainMenuLayer.h"

#include <iostream>
#include <SFML/Graphics.hpp>

namespace render {

    MainMenuLayer::MainMenuLayer()
    {
    }

    static sf::Font font;

    void MainMenuLayer::init()
    {
        // Load emptyMove texture
        textures.push_back(std::make_unique<sf::Texture>());

        // Load ticket textures
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/MainMenu_bg.png"))
            throw std::runtime_error("Failed to load MainMenu_bg.png");

        textures.push_back(std::make_unique<sf::Texture>());

        if (!textures.back()->loadFromFile(RESOURCE_DIR "/startButton.png"))
            throw std::runtime_error("Failed to load startButton.png");


        sprites.emplace_back(*textures[0]);
        sprites.emplace_back(*textures[1]);

        if (!font.loadFromFile(RESOURCE_DIR "/arial.ttf"))
            throw std::runtime_error("Failed to load font");

        /*texts.emplace_back("Start", font, 30);
        texts.back().setFillColor(sf::Color::White);*/
    }

    void MainMenuLayer::draw(sf::RenderTarget& target)
    {
        const sf::Vector2u textureSize = textures[0]->getSize();
        const sf::Vector2u windowSize = target.getSize();

        if (textureSize.x == 0 || textureSize.y == 0)
            return;

        const float scaleX =
            static_cast<float>(windowSize.x) / textureSize.x;

        const float scaleY =
            static_cast<float>(windowSize.y) / textureSize.y;

        float scale = std::max(scaleX, scaleY);


        sprites[0].setScale(scale,scale);

        sprites[0].setPosition(0,0);

        target.draw(sprites[0]);

        scale = 0.2f;

        sprites[1].setScale(scale,scale);

        sprites[1].setPosition(1200,700);

        target.draw(sprites[1]);

        /*texts[0].setPosition(1500,700);

        target.draw(texts[0]);*/
    }

    bool MainMenuLayer::isStartButtonClicked(sf::Vector2f position)
    {
        return sprites[1].getGlobalBounds().contains(position);
    }
}