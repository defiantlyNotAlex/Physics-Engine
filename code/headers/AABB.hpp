#pragma once
#include "Transform.hpp"
class AABB {
    public:
    AABB(Vector2f _min, Vector2f _max) {min = _min, max = _max;};
    AABB() {};

    Vector2f min;
    Vector2f max;

    bool const checkOverlap(AABB other);
    bool const checkPoint(Vector2f point);
};