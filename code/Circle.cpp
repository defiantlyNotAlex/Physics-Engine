#include "headers/Shape.hpp"

Circle::Circle(float _radius) : Shape(Shape::Type::Circle) {
    radius = _radius;
}  
float Circle::getRadius() {
    return radius;
}
const size_t Circle::getNormalVectors(Transform transform, vector<Vector2f>& out) {
    return 0;
}
const float Circle::getMaxProjection(Transform transform, Vector2f normal) {
    return VectorMaths::dotProd(transform.pos, normal) + radius;
}
const float Circle::getMinProjection(Transform transform, Vector2f normal) {
    return VectorMaths::dotProd(transform.pos, normal) - radius;
}
const vector<Edge> Circle::getEdges(Transform transform, Vector2f normal) {
    return {};
}
const bool Circle::checkPoint(Transform transform, Vector2f point) {
    return VectorMaths::magnitudeSqr(transform.pos - point) < radius * radius;
}
const AABB Circle::getBoundingBox(Transform transform) {
    return AABB(transform.pos - Vector2f(radius, radius), transform.pos + Vector2f(radius, radius));
}