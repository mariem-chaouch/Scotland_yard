#include "InfoLayer.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "state/MrX.h"

namespace render {

    InfoLayer::InfoLayer(const state::GameState& gameState):gameState(gameState){}

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



        if (!font.loadFromFile(RESOURCE_DIR "/arial.ttf"))
            throw std::runtime_error("Failed to load font");

        state::Player* currentplayer = gameState.getCurrentPlayer();
        for (int i = 0; i < 4; ++i)
        {
            int nbTicket = currentplayer->getNbOfTickets(static_cast<state::Ticket>(i));
            if (nbTicket != -1) {
                //Ticket counters
                sprites.emplace_back(*textures[1+i]);
                texts.emplace_back(std::to_string(nbTicket), font, 22);
                texts.back().setFillColor(sf::Color::White);
            }
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

        /*std::vector<std::pair<int, state::Ticket>> history;
        for (auto * player: gameState.getPlayers())
        {
            if (auto* mrX = dynamic_cast<state::MrX*>(player))
            {
                history = mrX->getHistory();
                break;
            }
        }*/
        // Temporary history for rendering tests
        static const std::vector<std::pair<int, state::Ticket>> history = {
            {15, state::Ticket::TaxiTicket},
            {23, state::Ticket::BusTicket},
            {45, state::Ticket::TrainTicket},
            {67, state::Ticket::BlackTicket},
            {89, state::Ticket::TaxiTicket},
            {102, state::Ticket::BusTicket},
            {120, state::Ticket::BlackTicket},
            {145, state::Ticket::TrainTicket}
        };
        for (int i = 0; i < 24; ++i)
        {
            std::size_t textureIndex = 0;

            if (static_cast<std::size_t>(i) < history.size())
            {
                switch (history[i].second)
                {
                case state::Ticket::TaxiTicket:
                    textureIndex = 1;
                    break;

                case state::Ticket::BusTicket:
                    textureIndex = 2;
                    break;

                case state::Ticket::TrainTicket:
                    textureIndex = 3;
                    break;

                case state::Ticket::BlackTicket:
                    textureIndex = 4;
                    break;
                }
            }
            // Update the texture only if it has changed
            if (sprites[i].getTexture() != textures[textureIndex].get())
            {
                sprites[i].setTexture(*textures[textureIndex], true);
            }
            const sf::Vector2u textureSize = sprites[i].getTexture()->getSize();

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


        for (std::size_t i = 0; i < texts.size(); ++i)
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
