#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "ECS/Components.h"

namespace Flow {
    class RenderSystem {
    public:
        static inline bool bDrawDebugColliders = false;

        static void Draw(entt::registry& registry, sf::RenderWindow& window) 
        {

            // 1. Draw Animated Entities (The Player)
            auto animView = registry.view<Transform, AnimatorComponent>();
            for (auto entity : animView) {
                auto& transform = animView.get<Transform>(entity);
                auto& anim = animView.get<AnimatorComponent>(entity);

                // Fetch the animation first
                auto it = anim.animations.find(anim.currentState);
                if (it == anim.animations.end() || !it->second.texture) continue;

                Animation& currentAnim = it->second;
                sf::Sprite sprite(*currentAnim.texture);

                sf::IntRect rect = currentAnim.frames[anim.currentFrame];
                sprite.setTextureRect(rect);

                if (anim.bFlipX) {
                    sprite.setScale({ -1.0f, 1.0f });
                    sprite.setOrigin({ (float)rect.size.x, 0.0f }); // Use .size.x
                }
                else {
                    sprite.setScale({ 1.0f, 1.0f });
                    sprite.setOrigin({ 0.0f, 0.0f });
                }

                sprite.setPosition({ transform.x, transform.y });
                window.draw(sprite);
            }

            // 2. Draw Static Geometry (Floors and Platforms)
            // We draw everything that has a BoxCollider but NO AnimatorComponent
            auto staticView = registry.view<Transform, BoxCollider>();
            for (auto entity : staticView) {
                if (registry.any_of<AnimatorComponent>(entity)) continue;

                auto& transform = staticView.get<Transform>(entity);
                auto& box = staticView.get<BoxCollider>(entity);

                sf::RectangleShape rect({ box.width, box.height });
                rect.setFillColor(sf::Color::Green);
                rect.setPosition({ transform.x + box.offsetX, transform.y + box.offsetY });

                window.draw(rect);
            }

            // NEW: Debug Collider Rendering
            if (bDrawDebugColliders)
            {
                auto debugView = registry.view<Transform, BoxCollider>();
                for (auto entity : debugView) {
                    auto& transform = debugView.get<Transform>(entity);
                    auto& box = debugView.get<BoxCollider>(entity);

                    sf::RectangleShape outline({ box.width, box.height });
                    outline.setFillColor(sf::Color::Transparent); // Empty inside

                    // Red for player, Yellow for everything else
                    if (registry.any_of<PlayerController>(entity) || registry.any_of<PlatformerController>(entity)) {
                        outline.setOutlineColor(sf::Color::Red);
                    }
                    else {
                        outline.setOutlineColor(sf::Color::Yellow);
                    }
                    outline.setOutlineThickness(1.0f);
                    outline.setPosition({ transform.x + box.offsetX, transform.y + box.offsetY });

                    window.draw(outline);
                }
            }

        }
    };
}