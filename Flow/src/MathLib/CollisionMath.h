#pragma once
#include <cmath>

namespace Flow {
    class CollisionMath {
    public:
        // AABB (Axis-Aligned Bounding Box) Collision
        static bool CheckAABB(float x1, float y1, float w1, float h1,
            float x2, float y2, float w2, float h2)
        {
            return (x1 < x2 + w2 &&
                x1 + w1 > x2 &&
                y1 < y2 + h2 &&
                y1 + h1 > y2);
        }

        // Circle-to-Circle Collision using squared distance (faster than sqrt)
        static bool CheckCircle(float x1, float y1, float r1,
            float x2, float y2, float r2)
        {
            float dx = x1 - x2;
            float dy = y1 - y2;
            float distanceSquared = (dx * dx) + (dy * dy);
            float radiusSum = r1 + r2;

            return distanceSquared < (radiusSum * radiusSum);
        }
    };
}