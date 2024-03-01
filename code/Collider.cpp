#include "headers/Collider.hpp"

Collider::Collider(Node* _parent, Transform _transform, ColliderType _colliderType) : Node (_parent, _transform) {
    colliderType = _colliderType;
}
Collider::~Collider() {}

inline ColliderType Collider::getType() {
    return colliderType;
}
inline const Vector2f Collider::getPosition() {
    return transform.pos;
}
inline const float Collider::getRotation() {
    return transform.rot;
}
Vector2f Collider::getMin() {
    return min;
}
Vector2f Collider::getMax() {
    return max;
}

void Collider::setPosition(Vector2f pos) {
    transform.pos = pos;
    updateBounds();
}
void Collider::setRotation(float rot) {
    transform.rot = rot;
    updateBounds();
}


bool Collider::inBounds(Vector2f point) {
    Vector2f thisMin = getMin();
    Vector2f thisMax = getMax();
    return point.x > thisMin.x && point.x < thisMax.x && point.y > thisMin.y && point.y < thisMax.y;
}
bool Collider::overlappingBounds(Collider * col) {
    const Vector2f otherMin = col->getMin();
    const Vector2f otherMax = col->getMax(); 

    const Vector2f thisMin = getMin();
    const Vector2f thisMax = getMax();
    
    return thisMin.x < otherMax.x
        && thisMax.x > otherMin.x
        && thisMin.y < otherMax.y
        && thisMax.y > otherMin.y;
}
bool Collider::checkCol(Collider* o_col) {
    if (!overlappingBounds(o_col)) return false;

    vector<Vector2f> dirVectors;
    auto displacement = VectorUtils::normalise(o_col->getPosition() - getPosition());
    dirVectors.push_back(displacement);

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