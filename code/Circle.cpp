#include "headers/Shape.hpp"

Circle::Circle(float _radius) {
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
std::array<float, 2> Circle::getProjection(Transform transform, Vector2f normal) const {
    float proj = Maths::dotProd(transform.pos, normal);
    return {proj - radius, proj + radius};
}
vector<Vector2f> Circle::getFeatures(Transform transform) const {
    return {transform.pos};
}
void Circle::getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const {
    for (Vector2f v : otherFeatures) {
        out.push_back(Maths::normalise(transform.pos - v));
    }
}
bool Circle::checkPoint(Transform transform, Vector2f point) const {
    return Maths::magnitudeSqr(transform.pos - point) <= radius * radius;
}
AABB Circle::getBoundingBox(Transform transform) const {
    return AABB(transform.pos - Vector2f(radius, radius), transform.pos + Vector2f(radius, radius));
}