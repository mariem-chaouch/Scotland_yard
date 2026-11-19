#include "PlayersLayer.h"
#include "state/Detective.h"
#include <SFML/Graphics.hpp>

namespace render {

    PlayersLayer::PlayersLayer(const state::GameState& gameState):gameState(gameState)
    {
    }

    void PlayersLayer::init()
    {
        // TODO: Load player textures.
    }

    void PlayersLayer::draw(sf::RenderTarget& target)
    {
        for (const auto* player : gameState.getPlayers()) {

        if (player == nullptr) {
            continue;
        }

        // Afficher uniquement les détectives
        if (dynamic_cast<const state::Detective*>(player)
            == nullptr) {
            continue;
        }

        int stationId = player->getCurrentStationId();

        // Convertir la couleur string en sf::Color
        sf::Color color = sf::Color::Blue; // Temporaire

        drawPawn(target, stationId, color);
    }
    }

    void PlayersLayer::drawPawn(sf::RenderTarget & target, int stationId, sf::Color color)
    {
        const auto& board = gameState.getBoard();

        const auto& station =
            board.getStation(stationId);

        const auto position = station.getPosition();
        constexpr float pawnRadius = 10.f;

        sf::CircleShape pawn(pawnRadius);

        pawn.setOrigin(pawnRadius, pawnRadius);

        pawn.setPosition(
            static_cast<float>(position.x),
            static_cast<float>(position.y)
        );

        pawn.setFillColor(color);
        pawn.setOutlineThickness(2.f);
        pawn.setOutlineColor(sf::Color::Black);

        target.draw(pawn);

    }
}
    