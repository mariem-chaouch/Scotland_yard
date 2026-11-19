#include "Scene.h"

#include "BoardLayer.h"
#include "InfoLayer.h"
#include "PlayersLayer.h"

namespace render {
    Scene::Scene(state::GameState& gameState):
        gameState(gameState),
    window(sf::VideoMode::getDesktopMode(),
   "Scotland Yard",
   sf::Style::Fullscreen) {
        //window.setFramerateLimit(60);
        layers.push_back(std::make_unique<BoardLayer>(gameState.getBoard()));
        layers.push_back(std::make_unique<PlayersLayer>(gameState));
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
            }
            drawScene();
        }
    }
}
