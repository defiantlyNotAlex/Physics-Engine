#include "headers/Shape.hpp"


Shape::Shape(Shape::Type _type) {
    type = _type;
}

Shape::Type Shape::getType() const{
    return type;
}

vector<Vector2f> Shape::getPoints() const{
    return {};
}