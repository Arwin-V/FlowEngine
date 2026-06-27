#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "ECS/Components.h"
#include "Input/InputTypes.h"
#include "Input/StringHash.h"


namespace Flow {
    class PhysicsSystem {
    public:
        // A static function means we don't need to create a "PhysicsSystem" object.
        // We just run the math directly.
        static void Update(entt::registry& registry, const std::vector<GameAction>& actions, float deltaTime) {

            // 1. Pre-process Input
            bool bIsThrustPressed = false;
            for (const auto& action : actions) {
                if (action.ActionID == Flow::HashString("P1_THRUST")) {
                    if (action.State == ActionState::Pressed || action.State == ActionState::Held) {
                        bIsThrustPressed = true;
                    }
                }
            }

            // 2. Apply Physics Logic
            auto view = registry.view<Transform, Velocity>();
            for (auto entity : view) {
                auto& transform = view.get<Transform>(entity);
                auto& velocity = view.get<Velocity>(entity);

                if (bIsThrustPressed) {
                    velocity.vx = 200.0f;
                }
                else {
                    velocity.vx = 0.0f;
                }

                transform.x += velocity.vx * deltaTime;
            }
        }
    };
}