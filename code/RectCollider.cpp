#include "headers/RectCollider.hpp"
#include <iostream>
using std::cout;
using std::endl;

RectCollider::RectCollider(Node* _parent, Transform _transform, Vector2f _size) : Collider(_parent, _transform, ColliderType::Rect) {
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

bool RectCollider::checkPoint(Vector2f point) {
    //if (!inBounds(point)) return false;

    for (size_t i = 0; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i]);
        auto curr = transform.convertLocaltoWorld(points[(i+1)%points.size()]);
        Vector2f side = curr - prev;
        Vector2f displacement = point - prev;
        if (VectorUtils::crossProd(displacement, side) > 0) {
            return false;
        }
    }
    return true;
}

bool RectCollider::overlapRect(RectCollider* r_col) {
    if (!overlappingBounds(r_col->getPosition(), r_col->getBounds())) return false;
    vector<Vector2f> dirVectors;
    auto displacement = VectorUtils::normalise(r_col->getPosition() - getPosition());
    auto perpDisplacement = Vector2f(-displacement.y, displacement.x);
    dirVectors.push_back(perpDisplacement);
    auto res = getSideVectors();
    dirVectors.insert(dirVectors.end(), res.begin(), res.end());
    res = r_col->getSideVectors();
    dirVectors.insert(dirVectors.end(), res.begin(), res.end());

    for (Vector2f dirVector : dirVectors) {
        float thisMin = FLT_MAX;
        float thisMax = -FLT_MAX;

        float otherMin = FLT_MAX;
        float otherMax = -FLT_MAX;
        
        getMaxProjection(dirVector, thisMin, thisMax);
        r_col->getMaxProjection(dirVector, otherMin, otherMax);
        
        if (!(thisMin < otherMax && thisMax > otherMin)) return false;
    }
    return true;
}

vector<Vector2f> RectCollider::getSideVectors() {
    return {VectorUtils::directionVector(transform.rot), VectorUtils::directionVector(transform.rot + M_PI_2)};
}
void RectCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    for (auto point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        auto projection = VectorUtils::projectScaler(t_point, directionVector);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}