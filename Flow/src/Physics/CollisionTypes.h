
#pragma once

namespace Flow
{
    // IDENTITY: What kind of object is this?
    enum class CollisionChannel
    {
        WorldStatic, // Walls, Floors
        Player,      // The Main Character
        Projectile,  // Bullets, Spells
        Enemy,       // AI
        MAX_CHANNELS // Used to size our arrays automatically
    };

    // ATTITUDE: How do we respond to a hit?
    enum class CollisionResponse
    {
        Ignore,  // Do no math, phase right through
        Overlap, // Phase through, but send a trigger event
        Block    // Physically halt movement
    };
}