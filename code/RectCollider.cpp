#include "headers/RectCollider.hpp"
#include <iostream>
using std::cout;
using std::endl;

RectCollider::RectCollider(Transform _transform, Vector2f _size) : Collider(_transform, ColliderType::Rect) {
    size = _size;
    Vector2f halfSize = size/2.f;
    points.push_back(Vector2f (-halfSize.x, -halfSize.y));
    points.push_back(Vector2f (halfSize.x, -halfSize.y));
    points.push_back(Vector2f (halfSize.x, halfSize.y));
    points.push_back(Vector2f (-halfSize.x, halfSize.y));
}

Vector2f RectCollider::getSize() {
    return size;
}

Vector2f RectCollider::getBounds() {
    Vector2f bounds = VectorUtils::zero();
    for (Vector2f point : points) {
        bounds.x = std::max(bounds.x, std::abs(point.x));
        bounds.y = std::max(bounds.y, std::abs(point.y));
    }
    return bounds;
}

vector<Vector2f> RectCollider::getPoints() {
    return points;
}

bool RectCollider::checkPoint(Vector2f point) {
    if (!inBounds(point)) return false;

    const Vector2f tranformedPos = transform.convertWorldtoLocal(point);
    const Vector2f halfSize = size/2.f;

    return tranformedPos.x > halfSize.x && tranformedPos.x < halfSize.x && tranformedPos.y > halfSize.y && tranformedPos.y < halfSize.y;
}

bool RectCollider::checkCol(Collider * col) {
    if (!overlappingBounds(col->getPosition(), col->getBounds())) return false;

    switch (col->getType()) {
    case ColliderType::None:
        return false;
    case ColliderType::Point:
        return checkPoint(col->getPosition());
    case ColliderType::Rect: 
        
        return false;
    case ColliderType::Circle:
        
        return false;
    case ColliderType::Polygon:

        return false;
    default:
        break;
    }
    return false;
}

bool RectCollider::overlapRect(RectCollider* r_col) {
    if (!overlappingBounds(r_col->getPosition(), r_col->getBounds())) return false;

    vector<Vector2f> dirVectors;
    dirVectors.push_back(VectorUtils::directionVector(transform.rot));
    dirVectors.push_back(VectorUtils::directionVector(r_col->transform.rot));
    dirVectors.push_back(VectorUtils::directionVector(transform.rot + M_PI_2));
    dirVectors.push_back(VectorUtils::directionVector(r_col->transform.rot + M_PI_2));

    for (Vector2f dirVector : dirVectors) {
        float thisMin = FLT_MAX;
        float thisMax = -FLT_MAX;

        float otherMin = FLT_MAX;
        float otherMax = -FLT_MAX;
        for (auto point : points) {
            auto t_point = transform.convertLocaltoWorld(point);
            thisMin = std::min(thisMin, VectorUtils::projectScaler(t_point, dirVector));
            thisMax = std::max(thisMax, VectorUtils::projectScaler(t_point, dirVector));
        }
        for (auto point : r_col->getPoints()) {
            auto t_point = r_col->transform.convertLocaltoWorld(point);
            otherMin = std::min(otherMin, VectorUtils::projectScaler(t_point, dirVector));
            otherMax = std::max(otherMax, VectorUtils::projectScaler(t_point, dirVector));
        }
        
        if (!(thisMin < otherMax && thisMax > otherMin)) return false;
    }
    return true;
}