#include "Scene.h"

#include <iostream>

#include "BoardLayer.h"
#include "InfoLayer.h"
#include "MainMenuLayer.h"
#include "PlayersLayer.h"

namespace render {
    Scene::Scene(state::GameState& gameState):
        gameState(gameState),
    window(sf::VideoMode::getDesktopMode(),
   "Scotland Yard",
   sf::Style::Fullscreen) {
        //window.setFramerateLimit(60);
        auto boardLayer = std::make_unique<BoardLayer>(gameState.getBoard());
        BoardLayer* boardLayerPtr = boardLayer.get();
        layers.push_back(std::make_unique<MainMenuLayer>());
        /*layers.push_back(std::move(boardLayer));
        layers.push_back(std::make_unique<PlayersLayer>(gameState, *boardLayerPtr));
        layers.push_back(std::make_unique<InfoLayer>(gameState));*/
    }

    void Scene::initScene()
    {
        for (auto& layer : layers) {
            layer->init();
        }
    }

    void Scene::drawScene()
    {
        window.clear(sf::Color(170,170,170));

        for (auto& layer : layers) {
            layer->draw(window);
        }

        window.display();
    }
    void Scene::runScene()
    {
        initScene();
        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                /*if (event.type == sf::Event::Resized)
                {
                    constexpr float aspectRatio = 16.f / 9.f;

                    unsigned int width = event.size.width;
                    unsigned int height = event.size.height;

                    if (static_cast<float>(width) / height > aspectRatio)
                        width = static_cast<unsigned int>(height * aspectRatio);
                    else
                        height = static_cast<unsigned int>(width / aspectRatio);

                    window.setSize({width, height});
                }*/
                if (event.type == sf::Event::Resized)
                {
                    sf::FloatRect visibleArea(
                        0.f, 0.f,
                        static_cast<float>(event.size.width),
                        static_cast<float>(event.size.height)
                    );

                    window.setView(sf::View(visibleArea));
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i pixelPosition(
                        event.mouseButton.x,
                        event.mouseButton.y
                    );

                    sf::Vector2f mouseWorld =
                        window.mapPixelToCoords(pixelPosition);

                    // Check if we are in the main menu
                    auto* menuLayer = dynamic_cast<MainMenuLayer*>(layers[0].get());

                    if (menuLayer)
                    {
                        if (menuLayer->isStartButtonClicked(mouseWorld))
                        {
                            startGame();
                        }
                    }
                    else
                    {
                        // We are in the game
                        auto* boardLayer = dynamic_cast<BoardLayer*>(layers[0].get());

                        if (boardLayer)
                        {
                            int transport = boardLayer->getTransportFromSelection(mouseWorld);

                            if (transport != -1)
                            {
                                switch (transport)
                                {
                                    case 0:
                                        std::cout << "Taxi selected!" << std::endl;
                                        break;

                                    case 1:
                                        std::cout << "Train selected!" << std::endl;
                                        break;

                                    case 2:
                                        std::cout << "Bus selected!" << std::endl;
                                        break;
                                }
                            }
                            else
                            {
                                boardLayer->selectStationAt(mouseWorld);
                            }
                        }
                    }
                }
            }
            drawScene();
        }
    }

    void Scene::startGame()
    {
        layers.clear();

        auto boardLayer = std::make_unique<BoardLayer>(gameState.getBoard());
        BoardLayer* boardLayerPtr = boardLayer.get();

        layers.push_back(std::move(boardLayer));

        layers.push_back(
            std::make_unique<PlayersLayer>(gameState, *boardLayerPtr)
        );

        layers.push_back(
            std::make_unique<InfoLayer>(gameState)
        );

        initScene();
    }
}
