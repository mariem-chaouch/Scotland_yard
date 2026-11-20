
#include <iostream>
#include <string>
#include <stdexcept>

#include "PlayersLayer.h"
#include "state/Detective.h"

#include <SFML/Graphics.hpp>

namespace render {

PlayersLayer::PlayersLayer(
    const state::GameState& gameState,
    const BoardLayer& boardLayer
)
    : gameState(gameState),
      boardLayer(boardLayer)
{
}

void PlayersLayer::init()
{
    // TODO: Load player textures.
}

void PlayersLayer::draw(sf::RenderTarget& target)
{
    const auto& board = gameState.getBoard();
    sf::FloatRect bounds = boardLayer.getMapBounds();

    // ========================================
    // AFFICHAGE DES DETECTIVES
    // ========================================

    for (auto* player : gameState.getPlayers())
    {
        if (player == nullptr)
            continue;

        // Ne pas afficher Mister X
        if (dynamic_cast<const state::Detective*>(player)
            == nullptr)
            continue;

        int stationId = player->getCurrentStationId();

        sf::Color color = sf::Color::White;
        std::string playerColor = player->getColor();

        if (playerColor == "blue")
            color = sf::Color::Blue;
        else if (playerColor == "red")
            color = sf::Color::Red;
        else if (playerColor == "green")
            color = sf::Color::Green;
        else if (playerColor == "yellow")
            color = sf::Color::Yellow;

        drawPawn(target, stationId, color);
    }
}

void PlayersLayer::drawPawn(
    sf::RenderTarget& target,
    int stationId,
    sf::Color color
)
{
    const auto& station =
        gameState.getBoard().getStation(stationId);

    const auto position = station.getPosition();

    sf::FloatRect bounds = boardLayer.getMapBounds();

    float x = bounds.left
            + position.x * bounds.width;

    float y = bounds.top
            + position.y * bounds.height;

    constexpr float radius = 10.f;

    sf::CircleShape pawn(radius);

    pawn.setOrigin(radius, radius);
    pawn.setPosition(x, y);

    pawn.setFillColor(color);
    pawn.setOutlineThickness(2.f);
    pawn.setOutlineColor(sf::Color::Black);

    target.draw(pawn);
}

} // namespace render
