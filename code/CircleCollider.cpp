#include "headers/CircleCollider.hpp"

CircleCollider::CircleCollider(Node* _parent, Transform _transform, float _radius) : Collider(_parent, _transform, ColliderType::Circle) {
    radius = _radius;
    updateBounds();
}
void CircleCollider::updateBounds() {
    min = transform.pos - Vector2f(radius, radius);
    max = transform.pos + Vector2f(radius, radius);
}

float CircleCollider::getRadius() {
    return radius;
}

bool CircleCollider::checkPoint(Vector2f point) {
    return VectorUtils::magnitudeSqr(transform.pos - point) < radius * radius;
}
size_t CircleCollider::getNormalVectors(vector<Vector2f>& out) {
    return 0;
}
void CircleCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    float projection = VectorUtils::projectScaler(transform.pos, directionVector);
    min = projection - radius;
    max = projection + radius;
}

size_t CircleCollider::getSupportPoints(Vector2f dir, vector<Vector2f>& support) {
    support.push_back({dir * radius + transform.pos});
    return 1;
}
Edge CircleCollider::getBestEdge(Vector2f normal) {
    const Vector2f point = transform.pos + normal * radius;
    return Edge(point, point, point);
}