#pragma once
#include <entt/entt.hpp>
#include "ECS/Components.h"
#include "Physics/CollisionMath.h"
#include "Utils/Log.h"

namespace Flow
{
    class CollisionSystem
    {
    public:
        static void Update(entt::registry& registry)
        {
            // 1. View only entities with Transform and BoxCollider
            auto view = registry.view<Transform, BoxCollider>();
            auto entities = view.begin();
            auto end = view.end();

            for (auto itA = entities; itA != end; ++itA)
            {
                auto entityA = *itA;
                auto& transformA = view.get<Transform>(entityA);
                auto& boxA = view.get<BoxCollider>(entityA);

                // Safely attempt to get velocity (returns nullptr if entity has no Velocity)
                auto* velA = registry.try_get<Velocity>(entityA);

                auto itB = itA;
                ++itB;
                for (; itB != end; ++itB)
                {
                    auto entityB = *itB;
                    auto& transformB = view.get<Transform>(entityB);
                    auto& boxB = view.get<BoxCollider>(entityB);

                    // Cross-reference responses
                    CollisionResponse responseA = boxA.responses[(int)boxB.objectType];
                    CollisionResponse responseB = boxB.responses[(int)boxA.objectType];

                    if (responseA == CollisionResponse::Ignore || responseB == CollisionResponse::Ignore)
                    {
                        continue;
                    }

                    // Calculate absolute positions
                    float aX = transformA.x + boxA.offsetX;
                    float aY = transformA.y + boxA.offsetY;
                    float bX = transformB.x + boxB.offsetX;
                    float bY = transformB.y + boxB.offsetY;

                    // Execute the Math
                    if (CollisionMath::CheckAABB(aX, aY, boxA.width, boxA.height, bX, bY, boxB.width, boxB.height))
                    {
                        if (responseA == CollisionResponse::Block && responseB == CollisionResponse::Block)
                        {
                            // 1. Fetch RigidBodies if they exist
                            auto* rbA = registry.try_get<RigidBody>(entityA);
                            auto* rbB = registry.try_get<RigidBody>(entityB);

                            // 2. Determine if they are static (No RigidBody = Static by default)
                            bool isAStatic = (rbA == nullptr || rbA->type == BodyType::Static);
                            bool isBStatic = (rbB == nullptr || rbB->type == BodyType::Static);

                            // 3. Resolve A (Push A away from B if A is dynamic/kinematic)
                            if (!isAStatic)
                            {
                                CollisionMath::ResolveAABB(transformA, boxA, transformB, boxB, velA);
                            }

                            // 4. Resolve B (Push B away from A if B is dynamic/kinematic)
                            if (!isBStatic)
                            {
                                auto* velB = registry.try_get<Velocity>(entityB);
                                CollisionMath::ResolveAABB(transformB, boxB, transformA, boxA, velB);
                            }

                            FLOW_LOG_WARN("Collision Blocked & Resolved between " << (uint32_t)entityA << " and " << (uint32_t)entityB);
                        }
                        else
                        {
                            FLOW_LOG_INFO("Overlap Detected between " << (uint32_t)entityA << " and " << (uint32_t)entityB);
                        }

                    }
                }
            }
        }
    };
}