// Flow/src/Physics/CollisionMath.h
#pragma once
#include <cmath>
#include <algorithm>
#include "ECS/Components.h"

namespace Flow
{
    class CollisionMath
    {
    public:
        // Basic Overlap Check (Boolean)
        static bool CheckAABB(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
        {
            return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
        }

        // NEW: Physical Resolution (Push-Back)
        static void ResolveAABB(Transform& tA, const BoxCollider& bA,
            const Transform& tB, const BoxCollider& bB, Velocity* vA)
        {
            // 1. Calculate Centers
            float aCenterX = tA.x + bA.offsetX + (bA.width / 2.0f);
            float aCenterY = tA.y + bA.offsetY + (bA.height / 2.0f);
            float bCenterX = tB.x + bB.offsetX + (bB.width / 2.0f);
            float bCenterY = tB.y + bB.offsetY + (bB.height / 2.0f);

            // 2. Calculate Half-Extents (Distance from center to edge)
            float aHalfW = bA.width / 2.0f;
            float aHalfH = bA.height / 2.0f;
            float bHalfW = bB.width / 2.0f;
            float bHalfH = bB.height / 2.0f;

            // 3. Distance between centers
            float dx = aCenterX - bCenterX;
            float dy = aCenterY - bCenterY;

            // 4. Calculate how deep we are intersecting on both axes
            float overlapX = (aHalfW + bHalfW) - std::abs(dx);
            float overlapY = (aHalfH + bHalfH) - std::abs(dy);

            // 5. Resolve on the axis of least penetration
            if (overlapX > 0 && overlapY > 0)
            {
                if (overlapX < overlapY)
                {
                    // Hit the Left or Right side
                    if (dx > 0) tA.x += overlapX; // Push Right
                    else        tA.x -= overlapX; // Push Left

                    if (vA) vA->vx = 0.0f;        // Stop X Velocity
                }
                else
                {
                    // Hit the Top or Bottom side
                    if (dy > 0) tA.y += overlapY; // Push Down
                    else        tA.y -= overlapY; // Push Up

                    if (vA) vA->vy = 0.0f;        // Stop Y Velocity
                }
            }
        }
    };
}