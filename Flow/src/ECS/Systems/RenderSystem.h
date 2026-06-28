// FILE: ./Flow/src/ECS/Systems/RenderSystem.h
#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include "ECS/Components.h"

namespace Flow
{
    class RenderSystem
    {
    public:
        static inline bool bDrawDebugColliders = false;

        static void Draw(entt::registry& Registry, sf::RenderWindow& Window)
        {
            // ---------------------------------------------------------
            // 1. CAMERA SYSTEM SETUP (Follow the Active Player)
            // ---------------------------------------------------------
            auto PlayerView = Registry.view<PlatformerController, Transform, BoxCollider>();

            if (PlayerView.begin() != PlayerView.end())
            {
                auto Entity = *PlayerView.begin();
                auto& pTrans = PlayerView.get<Transform>(Entity);
                auto& pBox = PlayerView.get<BoxCollider>(Entity);

                float CenterX = pTrans.x + (pBox.width / 2.0f);
                float CenterY = pTrans.y + (pBox.height / 2.0f);

                sf::View Camera(sf::Vector2f(CenterX, CenterY), sf::Vector2f(1280.0f, 720.0f));
                Window.setView(Camera);
            }

            // ---------------------------------------------------------
            // 2. BACKGROUND LAYER: STATIC WORLD TILES
            // ---------------------------------------------------------
            auto StaticView = Registry.view<Transform, BoxCollider>();
            for (auto Entity : StaticView)
            {
                // Skip rendering if this entity represents an animated character
                if (Registry.any_of<AnimatorComponent>(Entity))
                {
                    continue;
                }

                auto& pTrans = StaticView.get<Transform>(Entity);
                auto& pBox = StaticView.get<BoxCollider>(Entity);

                sf::RectangleShape RectShape({ pBox.width, pBox.height });
                RectShape.setFillColor(sf::Color::Green);
                RectShape.setPosition({ pTrans.x + pBox.offsetX, pTrans.y + pBox.offsetY });

                Window.draw(RectShape);
            }

            // ---------------------------------------------------------
            // 3. FOREGROUND LAYER: ANIMATED CHARACTERS & ACTORS
            // ---------------------------------------------------------
            auto AnimView = Registry.view<Transform, BoxCollider, AnimatorComponent>();
            for (auto Entity : AnimView)
            {
                auto& pTrans = AnimView.get<Transform>(Entity);
                auto& pBox = AnimView.get<BoxCollider>(Entity);
                auto& pAnim = AnimView.get<AnimatorComponent>(Entity);

                auto It = pAnim.animations.find(pAnim.currentState);
                if (It == pAnim.animations.end() || !It->second.texture)
                {
                    continue;
                }

                Animation& CurrentAnim = It->second;
                sf::Sprite Sprite(*CurrentAnim.texture);

                sf::IntRect Rect = CurrentAnim.frames[pAnim.currentFrame];
                Sprite.setTextureRect(Rect);

                // Set origin point to structural center of sprite textures
                Sprite.setOrigin({ Rect.size.x / 2.0f, Rect.size.y / 2.0f });

                if (pAnim.bFlipX)
                {
                    Sprite.setScale({ -1.0f, 1.0f });
                }
                else
                {
                    Sprite.setScale({ 1.0f, 1.0f });
                }

                // Compute screen coordinates centered over the collision volumes
                float BoxCenterX = pTrans.x + pBox.offsetX + (pBox.width / 2.0f);
                float BoxCenterY = pTrans.y + pBox.offsetY + (pBox.height / 2.0f);

                // Offset the asset vertically slightly so character feet align with ground lines
                Sprite.setPosition({ BoxCenterX, BoxCenterY - 10.0f });

                Window.draw(Sprite);
            }

            // ---------------------------------------------------------
            // 4. DIAGNOSTIC OVERLAY LAYER: DEBUG BOUNDING BOXES
            // ---------------------------------------------------------
            if (bDrawDebugColliders)
            {
                auto DebugView = Registry.view<Transform, BoxCollider>();
                for (auto Entity : DebugView)
                {
                    auto& pTrans = DebugView.get<Transform>(Entity);
                    auto& pBox = DebugView.get<BoxCollider>(Entity);

                    sf::RectangleShape Outline({ pBox.width, pBox.height });
                    Outline.setFillColor(sf::Color::Transparent);

                    if (Registry.any_of<PlatformerController>(Entity))
                    {
                        Outline.setOutlineColor(sf::Color::Red);
                    }
                    else
                    {
                        Outline.setOutlineColor(sf::Color::Yellow);
                    }

                    Outline.setOutlineThickness(1.0f);
                    Outline.setPosition({ pTrans.x + pBox.offsetX, pTrans.y + pBox.offsetY });

                    Window.draw(Outline);
                }
            }

            // ---------------------------------------------------------
            // 5. SCREENSPACE INITIALIZATION RESET
            // ---------------------------------------------------------
            Window.setView(Window.getDefaultView());
        }
    };
}