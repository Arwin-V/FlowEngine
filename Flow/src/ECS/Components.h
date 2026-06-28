#pragma once

#include "Physics/CollisionTypes.h"

namespace Flow 
{
    struct Transform 
    {
        float x = 0.0f;
        float y = 0.0f;
        float rotation = 0.0f;
        float scale = 1.0f;

     
    };

    struct Velocity
    {
        float vx = 0.0f;
        float vy = 0.0f;
    };

    struct BoxCollider
    {
        float width = 0.0f;
        float height = 0.0f;
        // Offset allows the designer to shift the hitbox away from the sprite's origin
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        // 1. The Identity of this collider
        CollisionChannel objectType = CollisionChannel::WorldStatic;

        // 2. The Attitude towards all other channels
        CollisionResponse responses[(int)CollisionChannel::MAX_CHANNELS];

        // Constructor: Default everything to 'Block' for safety 
        //1. Default Constructor(required for basic emplace)
        BoxCollider()
        {
            for (int i = 0; i < (int)CollisionChannel::MAX_CHANNELS; ++i)
            {
                responses[i] = CollisionResponse::Block;
            }
        }

        // 2. Custom Constructor (Matches your emplace<T>(entity, width, height) call)
        BoxCollider(float w, float h) : width(w), height(h)
        {
            for (int i = 0; i < (int)CollisionChannel::MAX_CHANNELS; ++i)
            {
                responses[i] = CollisionResponse::Block;
            }
        }

        // Helper function for the designer to easily set rules
        void SetResponse(CollisionChannel channel, CollisionResponse response)
        {
            responses[(int)channel] = response;
        }
    };

    struct CircleCollider 
    {
        float radius = 0.0f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
    };

    // Define how the engine should treat this entity's movement
    enum class BodyType
    {
        Static,    // Infinite mass, never moves (Walls, Floors)
        Kinematic, // Moved only by scripts/code, ignores gravity (Moving Platforms, Spaceships)
        Dynamic    // Fully controlled by the Physics System (Player, Boulders, Debris)
    };

    struct RigidBody
    {
        BodyType type = BodyType::Dynamic;

        float mass = 1.0f;

        // 1.0 = Normal gravity, 0.0 = Floating, -1.0 = Fall upwards
        float gravityScale = 1.0f;

        // Air resistance: Higher number = slows down faster (good for top-down games or water)
        float linearDrag = 0.0f;

        // We accumulate forces here before applying them to velocity
        float accumulatedForceX = 0.0f;
        float accumulatedForceY = 0.0f;

        // Designer Helper Function
        void AddForce(float forceX, float forceY)
        {
            accumulatedForceX += forceX;
            accumulatedForceY += forceY;
        }
 
    };

    // An empty tag to identify which entity listens to keyboard input
    struct PlayerController {};

    struct PlatformerController {
        // Movement configuration
        float moveSpeed = 400.0f;
        float acceleration = 2000.0f;
        float friction = 1500.0f;

        // Jump configuration
        float jumpForce = 600.0f;
        float fallMultiplier = 2.5f;     // Makes falling faster than rising (The "Mario" feel)
        float lowJumpMultiplier = 2.0f;  // If player releases jump button early

        // State tracking
        float velocityX = 0.0f;
        float velocityY = 0.0f;
        bool bIsGrounded = false;
        bool bIsJumping = false;
    };

    struct SpriteComponent 
    {
        std::shared_ptr<sf::Texture> Texture;
        sf::Sprite Sprite;
    };

    // Stores the sequence of image crops for a specific action
    struct Animation 
    {
        std::shared_ptr<sf::Texture> texture;
        std::vector<sf::IntRect> frames;
        float frameTime = 0.1f;
    };

    struct AnimatorComponent 
    {
        std::unordered_map<std::string, Animation> animations;
        std::string currentState = "Idle";
        int currentFrame = 0;
        float timer = 0.0f;
        bool bFlipX = false;
    };
}