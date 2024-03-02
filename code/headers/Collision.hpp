#pragma once
#include "VectorUtils.hpp"
struct CollisionManifold {
    public:
        Collider* other;
        Vector2f normal;
        Vector2f contact;
        float depth;
        bool exists;
        inline operator bool() const {return exists;};
};
