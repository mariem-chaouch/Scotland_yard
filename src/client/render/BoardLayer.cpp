#include "BoardLayer.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
using namespace std;
namespace render {

    /*void fitMapToWindow(const sf::RenderTarget& target)
    {

    }*/

    bool showSelection;
    sf::Vector2f selectionPosition;

    BoardLayer::BoardLayer(const state::Board& board)
        : board(board)
    {
    }

    void BoardLayer::init()
    {
        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/map.png")) {
            std::cerr << "Failed to load map.png\n";
            return;
        }
        sf::Vector2u size = textures.back()->getSize();

        std::cout << "Map size: "
                << size.x << " x " << size.y
                << std::endl;

        sprites.emplace_back(*textures[0]);

        //Selection Buttons
        showSelection = false;
        selectionPosition = {0.f,0.f};

        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/trainSelection.png"))
            throw std::runtime_error("Failed to load busMove.png");

        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/taxiSelection.png"))
            throw std::runtime_error("Failed to load busMove.png");

        textures.push_back(std::make_unique<sf::Texture>());
        if (!textures.back()->loadFromFile(RESOURCE_DIR "/busSelection.png"))
            throw std::runtime_error("Failed to load busMove.png");

        sprites.emplace_back(*textures[1]);
        sprites.emplace_back(*textures[2]);
        sprites.emplace_back(*textures[3]);
    }

    void BoardLayer::draw(sf::RenderTarget& target)
    {
        const int mapYoffset = 70;
        const sf::Vector2u textureSize = textures[0]->getSize();
        const sf::Vector2u windowSize = target.getSize();

        if (textureSize.x == 0 || textureSize.y == 0)
            return;

        const float scaleX =
            static_cast<float>(windowSize.x) / textureSize.x;

        const float scaleY =
            static_cast<float>(windowSize.y - mapYoffset) / textureSize.y;

        // Use the smaller scale to fit the entire map.
        const float scale = std::min(scaleX, scaleY);

        sprites[0].setScale(scale, scale);

        // Center the map.
        const float width = textureSize.x * scale;
        const float height = textureSize.y * scale;

        sprites[0].setPosition(
            (windowSize.x - width) / 2.f,
            (windowSize.y - mapYoffset - height) / 2.f
        );

        target.draw(sprites[0]);

        if (showSelection)
        {
            const float buttonSize = 120.f;

            for (int i = 0; i < 3; ++i)
            {
                const sf::Vector2u textureSize = textures[i + 1]->getSize();

                const float scale = buttonSize /
                    std::max(textureSize.x, textureSize.y);

                sprites[i + 1].setScale(scale, scale);

                sprites[i + 1].setPosition(
                    selectionPosition.x - buttonSize/2,
                    selectionPosition.y-buttonSize/2
                );

                target.draw(sprites[i + 1]);
            }
        }


    }

    sf::FloatRect BoardLayer::getMapBounds() const
    {
        if (sprites.empty()) {
            return sf::FloatRect();
        }

        return sprites[0].getGlobalBounds();
    }

    void BoardLayer::showSelectionAt(sf::Vector2f position)
    {
        showSelection = true;
        selectionPosition = position;
    }
}