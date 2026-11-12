#include "Scene.h"

#include "BoardLayer.h"
#include "InfoLayer.h"
#include "PlayersLayer.h"

namespace render {
    Scene::Scene(state::GameState& gameState):
        gameState(gameState),
        window(sf::VideoMode(1000, 600), "Scotland Yard") {
        //window.setFramerateLimit(60);
        layers.push_back(std::make_unique<BoardLayer>(gameState.getBoard()));
        layers.push_back(std::make_unique<PlayersLayer>());
        layers.push_back(std::make_unique<InfoLayer>());
    }

    void Scene::initScene()
    {
        for (auto& layer : layers) {
            layer->init();
        }
    }

    void Scene::drawScene()
    {
        window.clear(sf::Color::White);

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
            }
            drawScene();
        }
    }
}
