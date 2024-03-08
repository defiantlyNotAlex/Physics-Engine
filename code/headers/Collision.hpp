#pragma once
#include "VectorUtils.hpp"
#include <optional>
struct CollisionManifold {
    public:
        Vector2f normal;
        std::optional<Vector2f> contact;
        float depth;
};