#include "headers/Shape.hpp"

Circle::Circle(float _radius) {
    radius = _radius;
    centre = {{0.f, 0.f}};
}  
float Circle::getRadius() const {
    return radius;
}
void Circle::updateTransformedPoints(Transform transform) {
    centre[0] = transform.pos;
}
std::array<float, 2> Circle::getProjection(Transform transform, Vector2f normal) const {
    TIMERSTART();
    float proj = Maths::dotProd(transform.pos, normal);
    TIMEREND();
    return {proj - radius, proj + radius};
}
const vector<Vector2f>& Circle::getFeatures() const {
    return centre;
}
size_t Circle::getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const {
    TIMERSTART();
    float minDist;
    Vector2f min;
    for (size_t i = 0; i < otherFeatures.size(); i++) {
        Vector2f displacement = transform.pos - otherFeatures[i];
        float d = Maths::magnitudeSqr(displacement);
        if (i == 0 || d < minDist) {
            min = displacement;
            minDist = d;
        }
        
    }
    out.push_back(Maths::normalise(min));
    TIMEREND();
    return 1;
}
bool Circle::checkPoint(Transform transform, Vector2f point) const {
    return Maths::magnitudeSqr(transform.pos - point) <= radius * radius;
}
AABB Circle::getBoundingBox(Transform transform) const {
    return AABB(transform.pos - Vector2f(radius, radius), transform.pos + Vector2f(radius, radius));
}