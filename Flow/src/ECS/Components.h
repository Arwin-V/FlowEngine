// components.h
#pragma once

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
    };

    struct CircleCollider 
    {
        float radius = 0.0f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
    };

}