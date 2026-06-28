#pragma once
#include <entt/entt.hpp>
#include <vector>
#include "ECS/Components.h"
#include "Input/InputTypes.h"
#include "Input/StringHash.h"


namespace Flow {
    class PhysicsSystem {
    public:

        // A global gravity constant for the whole world (e.g., pulling down at 980 pixels/sec^2)
        static constexpr float WORLD_GRAVITY = 3000.0f;

        // A static function means we don't need to create a "PhysicsSystem" object.
        // We just run the math directly.
        static void Update(entt::registry& registry, const std::vector<GameAction>& actions, float deltaTime) 
        {

            // 1. Pre-process Input (Example: Adding force instead of setting velocity)
            bool bIsThrustPressed = false;
            for (const auto& action : actions)
            {
                if (action.ActionID == Flow::HashString("P1_THRUST"))
                {
                    if (action.State == ActionState::Pressed || action.State == ActionState::Held)
                    {
                        bIsThrustPressed = true;
                    }
                }
            }

            
            // 2. Physics Integration Loop
            // We now view Transform, Velocity, AND RigidBody
            auto view = registry.view<Transform, Velocity, RigidBody>();
            for (auto entity : view)
            {
                auto& transform = view.get<Transform>(entity);
                auto& velocity = view.get<Velocity>(entity);
                auto& rb = view.get<RigidBody>(entity);

                // Skip physics calculations entirely if the object is Static
                if (rb.type == BodyType::Static)
                {
                    continue;
                }

                if (rb.type == BodyType::Dynamic)
                {
                    // --- APPLY GRAVITY ---
                    // Force = Mass * Gravity * GravityScale
                    float gravityForce = rb.mass * WORLD_GRAVITY * rb.gravityScale;
                    rb.AddForce(0.0f, gravityForce);

                    // --- INPUT EXAMPLE ---
                    // Only apply thrust if this specific entity is tagged as the player
                    if (bIsThrustPressed && registry.any_of<PlayerController>(entity))
                    {
                        rb.AddForce(40000.0f, 0.0f); // Push Right
                    }

                    // --- CALCULATE ACCELERATION (a = F/m) ---
                    float accelerationX = rb.accumulatedForceX / rb.mass;
                    float accelerationY = rb.accumulatedForceY / rb.mass;

                    // --- INTEGRATE VELOCITY (v = v + a*dt) ---
                    velocity.vx += accelerationX * deltaTime;
                    velocity.vy += accelerationY * deltaTime;

                    // --- APPLY DRAG (Friction) ---
                    // Safely clamp the multiplier between 0 and 1 so high drag doesn't reverse velocity
                    float dragMultiplier = std::max(0.0f, 1.0f - (rb.linearDrag * deltaTime));
                    velocity.vx *= dragMultiplier;
                    velocity.vy *= dragMultiplier;

                    // CLEAR FORCES for the next frame
                    rb.accumulatedForceX = 0.0f;
                    rb.accumulatedForceY = 0.0f;
                }
                else if (rb.type == BodyType::Kinematic)
                {
                    // Kinematic bodies ignore gravity and forces. 
                    // They only move if a script explicitly set their velocity.
                    if (bIsThrustPressed)
                    {
                        velocity.vx = 200.0f;
                    }
                    else
                    {
                        velocity.vx = 0.0f;
                    }
                }

                // --- INTEGRATE POSITION (Move the Transform) ---
                transform.x += velocity.vx * deltaTime;
                transform.y += velocity.vy * deltaTime;
            }
        }
    };
}