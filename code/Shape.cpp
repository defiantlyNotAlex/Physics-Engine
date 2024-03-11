#include "headers/Shape.hpp"


Shape::Shape(Shape::Type _type) {
    type = _type;
}

Shape::Type const Shape::getType() {
    return type;
}

vector<Vector2f> Shape::getPoints() {
    return {};
}