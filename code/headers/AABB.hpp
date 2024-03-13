#pragma once
#include "Transform.hpp"
/// @brief Axis-Aligned Bounding Box
class AABB {
    public:
    AABB(Vector2f _min, Vector2f _max) {min = _min, max = _max;};
    AABB() {};

    Vector2f min;
    Vector2f max;

    /// @brief 
    /// @param other 
    /// @return 
    bool checkOverlap(AABB other) const;
    bool checkPoint(Vector2f point) const;
};