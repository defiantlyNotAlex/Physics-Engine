#include "headers/AABB.hpp"

bool AABB::checkOverlap (AABB other) const {
    return min.x < other.max.x
        && max.x > other.min.x
        && min.y < other.max.y
        && max.y > other.min.y;
}
bool AABB::checkPoint (Vector2f point) const {
    return point.x < max.x
        && point.x > min.x
        && point.y < max.y
        && point.y > min.y;
}