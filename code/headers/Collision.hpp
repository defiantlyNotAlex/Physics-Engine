#pragma once
#include "VectorMaths.hpp"
#include <optional>
struct CollisionManifold {
    public:
        // Collider* bodyA;
        // Collider* bodyB;
        // Vector2f relativeVelocity;
        // Vector2f[2] contacts;
        Vector2f normal;
        std::optional<Vector2f> contact;
        float depth;
};