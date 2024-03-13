#pragma once
#include "Transform.hpp"
class AABB {
    public:
    AABB(Vector2f _min, Vector2f _max) {min = _min, max = _max;};
    AABB() {};

    Vector2f min;
    Vector2f max;

    bool checkOverlap(AABB other) const;
    bool checkPoint(Vector2f point) const;
};