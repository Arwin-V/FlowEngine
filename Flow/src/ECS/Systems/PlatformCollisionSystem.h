// FILE: Flow/src/ECS/Systems/PlatformCollisionSystem.h
// ---------------------------------------------------
#pragma once
#include <entt/entt.hpp>
#include "ECS/Components.h"
#include "Physics/CollisionMath.h"

namespace Flow {
    class PlatformCollisionSystem {
    public:
        static void Update(entt::registry& registry, float deltaTime) {

            // 1. Get all players that need KCC movement
            auto playerView = registry.view<Transform, Velocity, PlatformerController, BoxCollider>();

            // 2. Get all static environment objects (Floors, Walls)
            auto staticView = registry.view<Transform, BoxCollider>();

            for (auto playerEntity : playerView) {
                auto& pTransform = playerView.get<Transform>(playerEntity);
                auto& pVel = playerView.get<Velocity>(playerEntity);
                auto& pCtrl = playerView.get<PlatformerController>(playerEntity);
                auto& pBox = playerView.get<BoxCollider>(playerEntity);

                // Reset grounded state at the start of the frame
                pCtrl.bIsGrounded = false;

                // ==========================================
                // STEP A: X-AXIS INTEGRATION & COLLISION
                // ==========================================
                pTransform.x += pVel.vx * deltaTime;

                for (auto staticEntity : staticView) {
                    if (playerEntity == staticEntity) continue;

                    // Filter: Only collide with Static World objects for the platforming foundation
                    auto* rb = registry.try_get<RigidBody>(staticEntity);
                    if (rb && rb->type != BodyType::Static) continue;

                    auto& sTransform = staticView.get<Transform>(staticEntity);
                    auto& sBox = staticView.get<BoxCollider>(staticEntity);

                    float pX = pTransform.x + pBox.offsetX;
                    float pY = pTransform.y + pBox.offsetY;
                    float sX = sTransform.x + sBox.offsetX;
                    float sY = sTransform.y + sBox.offsetY;

                    if (CollisionMath::CheckAABB(pX, pY, pBox.width, pBox.height, sX, sY, sBox.width, sBox.height)) {
                        // Resolve X overlap
                        if (pVel.vx > 0) { // Moving Right -> Snap to left edge of static box
                            pTransform.x = sX - (pBox.width + pBox.offsetX);
                        }
                        else if (pVel.vx < 0) { // Moving Left -> Snap to right edge
                            pTransform.x = (sX + sBox.width) - pBox.offsetX;
                        }
                        pVel.vx = 0.0f; // Kill horizontal momentum against the wall
                    }
                }

                // ==========================================
                // STEP B: Y-AXIS INTEGRATION & COLLISION
                // ==========================================
                pTransform.y += pVel.vy * deltaTime;

                for (auto staticEntity : staticView) {
                    if (playerEntity == staticEntity) continue;

                    auto* rb = registry.try_get<RigidBody>(staticEntity);
                    if (rb && rb->type != BodyType::Static) continue;

                    auto& sTransform = staticView.get<Transform>(staticEntity);
                    auto& sBox = staticView.get<BoxCollider>(staticEntity);

                    float pX = pTransform.x + pBox.offsetX;
                    float pY = pTransform.y + pBox.offsetY;
                    float sX = sTransform.x + sBox.offsetX;
                    float sY = sTransform.y + sBox.offsetY;

                    if (CollisionMath::CheckAABB(pX, pY, pBox.width, pBox.height, sX, sY, sBox.width, sBox.height)) {
                        // Resolve Y overlap
                        if (pVel.vy > 0) {
                            // Moving Down -> Hit a floor!
                            pTransform.y = sY - (pBox.height + pBox.offsetY);
                            pCtrl.bIsGrounded = true;
                            pCtrl.bIsJumping = false;
                        }
                        else if (pVel.vy < 0) {
                            // Moving Up -> Hit a ceiling (bonk!)
                            pTransform.y = (sY + sBox.height) - pBox.offsetY;
                        }
                        pVel.vy = 0.0f; // Kill vertical momentum
                    }
                }
            }
        }
    };
}