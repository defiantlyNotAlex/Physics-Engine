#include "headers/Collider.hpp"

Collider::Collider(Transform _transform, ColliderType _colliderType) {
    transform = _transform;
    _colliderType = _colliderType;
}
Collider::~Collider() {}
ColliderType Collider::getType() {
    return colliderType;
}
Vector2f Collider::getPosition() {
    return transform.pos;
}
Vector2f Collider::getMax() {
    return transform.pos + getBounds()/2.f;
}
Vector2f Collider::getMin() {
    return transform.pos - getBounds()/2.f;
}
bool Collider::inBounds(Vector2f point) {
    const Vector2f thisMin = getMin();
    const Vector2f thisMax = getMax();
    return point.x > thisMin.x && point.x < thisMax.x && point.y > thisMin.y && point.y < thisMax.y;
}
bool Collider::overlappingBounds(Vector2f otherPos, Vector2f otherBounds) {
    const Vector2f otherMin = otherPos - otherBounds/2.f;
    const Vector2f otherMax = otherPos + otherBounds/2.f; 

    const Vector2f thisMin = getMin();
    const Vector2f thisMax = getMax();
    
    return ((otherMin.x > thisMin.x || otherMin.x < thisMax.x) 
        && (otherMax.x > thisMin.x || otherMax.x < thisMax.x))
        && ((otherMin.y > thisMin.y || otherMin.y < thisMax.y)
        && (otherMax.y > thisMin.y || otherMax.y < thisMax.y));
}