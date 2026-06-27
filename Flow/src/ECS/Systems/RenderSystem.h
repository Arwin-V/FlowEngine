// Flow/src/ECS/Systems/RenderSystem.h
#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "ECS/Components.h"

namespace Flow {
    class RenderSystem {
    public:
        static void Draw(entt::registry& registry, sf::RenderWindow& window) {
            auto view = registry.view<Transform>();

            for (auto entity : view) {
                auto& transform = view.get<Transform>(entity);

                sf::CircleShape circle(20.0f);
                circle.setFillColor(sf::Color::Green);
                circle.setPosition({ transform.x, transform.y });

                window.draw(circle);
            }
        }
    };
}