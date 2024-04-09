#include "headers/Shape.hpp"

Circle::Circle(float _radius) : Shape(Shape::Type::Circle) {
    radius = _radius;
}  
float Circle::getRadius() const {
    return radius;
}
size_t Circle::getNormalVectors(Transform transform, vector<Vector2f>& out) const {
    return 0;
}
float Circle::getMaxProjection(Transform transform, Vector2f normal) const {
    return Maths::dotProd(transform.pos, normal) + radius;
}
float Circle::getMinProjection(Transform transform, Vector2f normal) const {
    return Maths::dotProd(transform.pos, normal) - radius;
}
vector<Edge> Circle::getEdges(Transform transform, Vector2f normal) const {
    return {};
}
bool Circle::checkPoint(Transform transform, Vector2f point) const {
    return Maths::magnitudeSqr(transform.pos - point) <= radius * radius;
}
AABB Circle::getBoundingBox(Transform transform) const {
    return AABB(transform.pos - Vector2f(radius, radius), transform.pos + Vector2f(radius, radius));
}