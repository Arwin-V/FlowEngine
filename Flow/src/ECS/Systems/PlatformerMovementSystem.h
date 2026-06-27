// FILE: Flow/src/ECS/Systems/PlatformMovementSystem.h
// ---------------------------------------------------
#pragma once
#include <entt/entt.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include "ECS/Components.h"
#include "Input/InputTypes.h"
#include "Input/StringHash.h"

namespace Flow {
    class PlatformMovementSystem {
    public:
        // Core Gravity Constant for the Platformer World
        static constexpr float PLATFORM_GRAVITY = 2000.0f;

        static void Update(entt::registry& registry, const std::vector<GameAction>& actions, float deltaTime)
        {
            // 1. Parse Inputs for the current frame
            bool bMoveLeft = false;
            bool bMoveRight = false;
            bool bJumpPressed = false;
            bool bJumpHeld = false;

            for (const auto& action : actions)
            {
                if (action.ActionID == Flow::HashString("P1_LEFT"))
                {
                    if (action.State == ActionState::Pressed || action.State == ActionState::Held) bMoveLeft = true;
                }
                else if (action.ActionID == Flow::HashString("P1_RIGHT"))
                {
                    if (action.State == ActionState::Pressed || action.State == ActionState::Held) bMoveRight = true;
                }
                else if (action.ActionID == Flow::HashString("P1_JUMP"))
                {
                    if (action.State == ActionState::Pressed) bJumpPressed = true;
                    if (action.State == ActionState::Pressed || action.State == ActionState::Held) bJumpHeld = true;
                }
            }

            // 2. Process all entities with a PlatformerController
            auto view = registry.view<Transform, Velocity, PlatformerController>();
            for (auto entity : view)
            {
                auto& transform = view.get<Transform>(entity);
                auto& velocity = view.get<Velocity>(entity);
                auto& controller = view.get<PlatformerController>(entity);

                // ==========================================
                // HORIZONTAL MOVEMENT (Snappy Acceleration)
                // ==========================================
                float targetVelocityX = 0.0f;
                if (bMoveLeft)  targetVelocityX -= controller.moveSpeed;
                if (bMoveRight) targetVelocityX += controller.moveSpeed;

                // Apply sharp acceleration or high friction (for snappy stops)
                if (targetVelocityX != 0.0f)
                {
                    // Accelerate towards target speed
                    velocity.vx = MoveTowards(velocity.vx, targetVelocityX, controller.acceleration * deltaTime);
                }
                else
                {
                    // Apply heavy friction to stop instantly like Mario
                    velocity.vx = MoveTowards(velocity.vx, 0.0f, controller.friction * deltaTime);
                }

                // ==========================================
                // VERTICAL MOVEMENT (Variable Jump & Gravity)
                // ==========================================

                // Jump Logic
                if (bJumpPressed && controller.bIsGrounded)
                {
                    velocity.vy = -controller.jumpForce; // Negative Y is UP in 2D
                    controller.bIsJumping = true;
                    controller.bIsGrounded = false;
                }

                // Custom Gravity Curves
                if (velocity.vy > 0)
                {
                    // Falling: Apply heavier gravity for a less "floaty" feel
                    velocity.vy += PLATFORM_GRAVITY * controller.fallMultiplier * deltaTime;
                }
                else if (velocity.vy < 0 && !bJumpHeld)
                {
                    // Short Hop: If moving up but button is released, cut the jump short
                    velocity.vy += PLATFORM_GRAVITY * controller.lowJumpMultiplier * deltaTime;
                }
                else
                {
                    // Standard Gravity
                    velocity.vy += PLATFORM_GRAVITY * deltaTime;
                }

      
            }
        }

    private:
        // Helper function for snappy, linear velocity changes
        static float MoveTowards(float current, float target, float maxDelta)
        {
            if (std::abs(target - current) <= maxDelta)
            {
                return target;
            }
            return current + std::copysign(maxDelta, target - current);
        }
    };
}