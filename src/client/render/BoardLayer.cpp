#include "BoardLayer.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
using namespace std;
namespace render {

    /*void fitMapToWindow(const sf::RenderTarget& target)
    {

    }*/

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

        sprites.emplace_back(*textures.back());
    }

    void BoardLayer::draw(sf::RenderTarget& target)
    {
        const int mapYoffset = 70;
        const sf::Vector2u textureSize = textures.back()->getSize();
        const sf::Vector2u windowSize = target.getSize();

        if (textureSize.x == 0 || textureSize.y == 0)
            return;

        const float scaleX =
            static_cast<float>(windowSize.x) / textureSize.x;

        const float scaleY =
            static_cast<float>(windowSize.y - mapYoffset) / textureSize.y;

        // Use the smaller scale to fit the entire map.
        const float scale = std::min(scaleX, scaleY);

        sprites.back().setScale(scale, scale);

        // Center the map.
        const float width = textureSize.x * scale;
        const float height = textureSize.y * scale;

        sprites.back().setPosition(
            (windowSize.x - width) / 2.f,
            (windowSize.y - mapYoffset - height) / 2.f
        );
      
        target.draw(sprites.back());
    }
    sf::FloatRect BoardLayer::getMapBounds() const
    {
        if (sprites.empty()) {
        return sf::FloatRect();
        }

        return sprites[0].getGlobalBounds();
        }

}