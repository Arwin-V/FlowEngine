// Flow/src/ECS/Systems/RenderSystem.h
#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "ECS/Components.h"

namespace Flow {
    class RenderSystem {
    public:
        static void Draw(entt::registry& registry, sf::RenderWindow& window) {
            // View entities that have BOTH Transform and BoxCollider
            auto view = registry.view<Transform, BoxCollider>();

            for (auto entity : view) {
                auto& transform = view.get<Transform>(entity);
                auto& box = view.get<BoxCollider>(entity);

                sf::RectangleShape rect({ box.width, box.height });
                rect.setPosition({ transform.x + box.offsetX, transform.y + box.offsetY });

                // If it's the player, make it RED. Otherwise, GREEN platforms.
                if (registry.any_of<PlatformerController>(entity)) {
                    rect.setFillColor(sf::Color::Red);
                }
                else {
                    rect.setFillColor(sf::Color::Green);
                }

                window.draw(rect);
            }
        }
    };
}