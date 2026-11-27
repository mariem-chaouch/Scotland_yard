#include "BoardLayer.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#define RADIUS 10.f

using namespace std;
namespace render {

    /*void fitMapToWindow(const sf::RenderTarget& target)
    {

    }*/

    bool showSelection;
    sf::Vector2f selectionPosition;
    sf::Font stationFont;

    BoardLayer::BoardLayer(const state::Board& board)
        : board(board)
    {
    }

    void BoardLayer::init()
    {
        if (!stationFont.loadFromFile(RESOURCE_DIR "/arial.ttf"))
            throw std::runtime_error("Failed to load station font");
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


        const auto& bounds = getMapBounds();
        int stationsFound = 0;
        int stationsMissing = 0;

        for (int id = 1; id < 200; ++id)
        {
            try
            {

                const auto& station = board.getStation(id);
                const auto position = station.getPosition();

                const float x = bounds.left + position.x * bounds.width;
                const float y = bounds.top + position.y * bounds.height;

                // Station circle
                sf::CircleShape point(RADIUS);
                point.setOrigin(RADIUS, RADIUS);
                point.setPosition(x, y);

                point.setFillColor(sf::Color::Red);
                point.setOutlineThickness(1.f);
                point.setOutlineColor(sf::Color::White);

                target.draw(point);

                // Station ID
                sf::Text stationId;
                stationId.setFont(stationFont);
                stationId.setString(std::to_string(id));
                stationId.setCharacterSize(12);
                stationId.setFillColor(sf::Color::White);

                // Center the text inside the circle
                sf::FloatRect textBounds = stationId.getLocalBounds();

                stationId.setOrigin(
                    textBounds.left + textBounds.width / 2.f,
                    textBounds.top + textBounds.height / 2.f
                );

                stationId.setPosition(x, y);

                target.draw(stationId);

                ++stationsFound;
            }
            catch (const std::invalid_argument&)
            {
                ++stationsMissing;
            }
        }

        // ========================================
        // TEST 3 : diagnostic dans le terminal
        // Affichage une seule fois
        // ========================================

        static bool diagnosticPrinted = false;

        if (!diagnosticPrinted)
        {
            std::cout << "===== TEST STATIONS =====" << std::endl;

            std::cout << "Stations trouvees : "
                      << stationsFound << std::endl;

            std::cout << "Stations absentes : "
                      << stationsMissing << std::endl;

            std::cout << "Map bounds : "
                      << bounds.left << ", "
                      << bounds.top << " / "
                      << bounds.width << " x "
                      << bounds.height << std::endl;

            std::cout << "=========================" << std::endl;

            diagnosticPrinted = true;
        }


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

    void showSelectionAt(sf::Vector2f position)
    {
        showSelection = true;
        selectionPosition = position;
    }

    void hideSelection()
    {
        showSelection = false;
    }

    bool BoardLayer::selectStationAt(sf::Vector2f clickPosition)
    {
        const auto bounds = getMapBounds();

        for (int id = 1; id < 200; ++id)
        {
            try
            {
                const auto& station = board.getStation(id);
                const auto position = station.getPosition();

                const float x = bounds.left + position.x * bounds.width;
                const float y = bounds.top + position.y * bounds.height;

                const float dx = clickPosition.x - x;
                const float dy = clickPosition.y - y;

                const float distance = std::sqrt(dx * dx + dy * dy);

                if (distance <= RADIUS)
                {
                    showSelectionAt({x, y});
                    return true;
                }
            }
            catch (const std::invalid_argument&)
            {
                // Station doesn't exist
            }
        }

        hideSelection();
        return false;
    }

    int BoardLayer::getTransportFromSelection(sf::Vector2f clickPosition)
    {
        if (!showSelection)
            return -1;

        const float buttonSize = 120.f;
        const float radius = buttonSize / 2.f;

        float dx = clickPosition.x - selectionPosition.x;
        float dy = clickPosition.y - selectionPosition.y;

        // Check if the click is inside the wheel
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > radius)
            return -1;

        // Calculate angle in degrees
        float angle = (std::atan2(dy, dx) * 180.f / 3.14159265f) + 90 ;

        // Convert negative angles to [0, 360)
        if (angle < 0)
            angle += 360.f;

        hideSelection();

        // Determine the selected transport
        if (angle >= 0.f && angle < 120.f)
            return 0; // Taxi

        if (angle >= 120.f && angle < 240.f)
            return 1; // Train

        return 2; // Bus

    }
}