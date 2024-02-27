#include "CircleCollider.hpp"

CircleCollider::CircleCollider(Transform _transform, float _radius) : Collider(_transform, ColliderType::Circle) {
    radius = _radius;
}
        
Vector2f CircleCollider::getBounds() {
    return Vector2f(radius, radius);
}

bool CircleCollider::checkPoint(Vector2f point) {
    return VectorUtils::magnitudeSqr(point - transform.pos) < radius * radius;
}
vector<Vector2f> CircleCollider::getSideVectors() {
    return {};
}
void CircleCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    float projection = VectorUtils::projectScaler(transform.pos, directionVector);
    min = projection - radius;
    max = projection + radius;
}