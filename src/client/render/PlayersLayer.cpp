
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
    // TEST 1 : cercle au centre de la carte
    // ========================================

    sf::CircleShape testPoint(15.f);
    testPoint.setOrigin(15.f, 15.f);

    testPoint.setPosition(
        bounds.left + bounds.width * 0.5f,
        bounds.top + bounds.height * 0.5f
    );

    testPoint.setFillColor(sf::Color::Magenta);
    target.draw(testPoint);

    // ========================================
    // TEST 2 : positions des stations
    // ========================================

    int stationsFound = 0;
    int stationsMissing = 0;

    for (int id = 1; id <= 200; ++id)
    {
        try
        {
            const auto& station = board.getStation(id);
            const auto position = station.getPosition();

            float x = bounds.left
                    + position.x * bounds.width;

            float y = bounds.top
                    + position.y * bounds.height;

            sf::CircleShape point(5.f);
            point.setOrigin(5.f, 5.f);
            point.setPosition(x, y);

            point.setFillColor(sf::Color::Red);
            point.setOutlineThickness(1.f);
            point.setOutlineColor(sf::Color::White);

            target.draw(point);

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
