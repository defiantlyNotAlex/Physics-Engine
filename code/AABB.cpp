#include "headers/AABB.hpp"

bool const AABB::checkOverlap (AABB other) {
    return min.x < other.max.x
        && max.x > other.min.x
        && min.y < other.max.y
        && max.y > other.min.y;
}
bool const AABB::checkPoint (Vector2f point) {
    return point.x < max.x
        && point.x > min.x
        && point.y < max.y
        && point.y > min.y;
}