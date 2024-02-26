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
bool Collider::checkCol(Collider* o_col) {
    if (!overlappingBounds(o_col->getPosition(), o_col->getBounds())) return false;
    vector<Vector2f> dirVectors;
    auto displacement = VectorUtils::normalise(o_col->getPosition() - getPosition());
    auto perpDisplacement = Vector2f(-displacement.y, displacement.x);
    dirVectors.push_back(perpDisplacement);
    auto res = getSideVectors();
    dirVectors.insert(dirVectors.end(), res.begin(), res.end());
    res = o_col->getSideVectors();
    dirVectors.insert(dirVectors.end(), res.begin(), res.end());

    for (Vector2f dirVector : dirVectors) {
        float thisMin = FLT_MAX;
        float thisMax = -FLT_MAX;

        float otherMin = FLT_MAX;
        float otherMax = -FLT_MAX;
        
        getMaxProjection(dirVector, thisMin, thisMax);
        o_col->getMaxProjection(dirVector, otherMin, otherMax);
        
        if (!(thisMin < otherMax && thisMax > otherMin)) return false;
    }
    return true;
}