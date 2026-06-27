#pragma once
#include <entt/entt.hpp>
#include "ECS/Components.h"
#include "MathLib/CollisionMath.h"
#include "Utils/Log.h"

namespace Flow 
{
    class CollisionSystem 
    {
    public:
        static void Update(entt::registry& registry) {
            // Get all entities that have both a Transform and a BoxCollider
            auto view = registry.view<Transform, BoxCollider>();

            // Convert view to an array-like structure for the nested loop
            auto entities = view.begin();
            auto end = view.end();

            for (auto itA = entities; itA != end; ++itA) {
                auto entityA = *itA;
                auto& transformA = view.get<Transform>(entityA);
                auto& boxA = view.get<BoxCollider>(entityA);

                // Start inner loop from the next entity to avoid checking A vs A, and checking A vs B twice
                auto itB = itA;
                ++itB;
                for (; itB != end; ++itB) {
                    auto entityB = *itB;
                    auto& transformB = view.get<Transform>(entityB);
                    auto& boxB = view.get<BoxCollider>(entityB);

                    // Calculate absolute positions (Transform + Offset)
                    float aX = transformA.x + boxA.offsetX;
                    float aY = transformA.y + boxA.offsetY;
                    float bX = transformB.x + boxB.offsetX;
                    float bY = transformB.y + boxB.offsetY;

                    // Execute the Math
                    if (CollisionMath::CheckAABB(aX, aY, boxA.width, boxA.height,
                        bX, bY, boxB.width, boxB.height))
                    {
                        // A collision happened!
                        FLOW_LOG_WARN("Collision detected between Entity " << (uint32_t)entityA << " and Entity " << (uint32_t)entityB);

                        // FUTURE: Send event or resolve physics here
                    }
                }
            }
        }
    };
}