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

}