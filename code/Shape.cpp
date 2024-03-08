#include "headers/Shape.hpp"

bool Box::OverlapBox (Box A, Box B) {
    return A.min.x < B.max.x
        && A.max.x > B.min.x
        && A.min.y < B.max.y
        && A.max.y > B.min.y;
}
bool const Box::checkPoint (Vector2f point) {
    return point.x < max.x
        && point.x > min.x
        && point.y < max.y
        && point.y > min.y;
}

Shape::Shape(Shape::Type _type) {
    type = _type;
}

Shape::Type const inline Shape::getType() {
    return type;
}
