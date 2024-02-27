#include "headers/CircleCollider.hpp"

CircleCollider::CircleCollider(Node* _parent, Transform _transform, float _radius) : Collider(_parent, _transform, ColliderType::Circle) {
    radius = _radius;
}
        
Vector2f CircleCollider::getBounds() {
    return Vector2f(radius, radius);
}

float CircleCollider::getRadius() {
    return radius;
}

bool CircleCollider::checkPoint(Vector2f point) {
    return VectorUtils::magnitudeSqr(transform.pos - point) < radius * radius;
}
vector<Vector2f> CircleCollider::getSideVectors() {
    return {};
}
void CircleCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    float projection = VectorUtils::projectScaler(transform.pos, directionVector);
    min = projection - radius;
    max = projection + radius;
}