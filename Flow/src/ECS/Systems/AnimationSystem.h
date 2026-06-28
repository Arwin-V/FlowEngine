#pragma once
#include <entt/entt.hpp>
#include <cmath>
#include "ECS/Components.h"

namespace Flow {
    class AnimationSystem {
    public:
        static void Update(entt::registry& registry, float deltaTime) {
            auto view = registry.view<AnimatorComponent, Velocity>();

            for (auto entity : view) {
                auto& anim = view.get<AnimatorComponent>(entity);
                auto& vel = view.get<Velocity>(entity);

                // 1. Auto-detect state based on physics velocity
                std::string nextState = (std::abs(vel.vx) > 10.0f) ? "Run" : "Idle";

                if (anim.currentState != nextState) {
                    anim.currentState = nextState;
                    anim.currentFrame = 0;
                    anim.timer = 0.0f;
                }

                // 2. Determine facing direction
                if (vel.vx < -1.0f) anim.bFlipX = true;
                else if (vel.vx > 1.0f) anim.bFlipX = false;

                // 3. Tick the animation timer
                auto& currentAnim = anim.animations[anim.currentState];
                anim.timer += deltaTime;
                if (anim.timer >= currentAnim.frameTime) {
                    anim.timer -= currentAnim.frameTime;
                    anim.currentFrame = (anim.currentFrame + 1) % currentAnim.frames.size();
                }
            }
        }
    };
}