#include "headers/PolygonCollider.hpp"

PolygonCollider::PolygonCollider(Node* _parent, Transform _transform, vector<Vector2f> _points) : Collider (_parent, _transform, ColliderType::Polygon) {
    points = _points;
}
PolygonCollider::~PolygonCollider() {}

Vector2f PolygonCollider::getBounds() {
    Vector2f bounds = {0, 0};
    for (auto point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        bounds.x = std::max(bounds.x, std::abs(t_point.x));
        bounds.y = std::max(bounds.y, std::abs(t_point.y));
    }
    return bounds;
}
bool PolygonCollider::checkPoint(Vector2f point) {\
    if (!inBounds(point)) return false;

    for (size_t i = 0; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i]);
        auto curr = transform.convertLocaltoWorld(points[i+1%points.size()]);
        Vector2f side =  prev - curr;
        Vector2f displacement = prev - point;
        if (VectorUtils::crossProd(side, displacement) < 0) {
            return false;
        }
    }
    return true;
}
vector<Vector2f> PolygonCollider::getSideVectors() {
    vector<Vector2f> sideVectors;
    for (size_t i = 1; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i-1]);
        auto curr = transform.convertLocaltoWorld(points[i]);
        sideVectors.push_back(curr - prev);
    }
    return sideVectors;
}
void PolygonCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    for (auto point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        auto projection = VectorUtils::projectScaler(t_point, directionVector);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}