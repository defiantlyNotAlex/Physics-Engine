#include "headers/PolygonCollider.hpp"

PolygonCollider::PolygonCollider(Node* _parent, Transform _transform, vector<Vector2f> _points) : Collider (_parent, _transform, ColliderType::Polygon) {
    points = _points;
}
PolygonCollider::~PolygonCollider() {}

vector<Vector2f>& PolygonCollider::getPoints() {
    return points;
}

Vector2f PolygonCollider::getMin() {
    Vector2f bounds = Vector2f(FLT_MAX, FLT_MAX);
    for (Vector2f point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        bounds.x = std::min(bounds.x, t_point.x);
        bounds.y = std::min(bounds.y, t_point.y);
    }
    return bounds;
}
Vector2f PolygonCollider::getMax() {
    Vector2f bounds = Vector2f(FLT_MIN, FLT_MIN);
    for (Vector2f point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        bounds.x = std::max(bounds.x, t_point.x);
        bounds.y = std::max(bounds.y, t_point.y);
    }
    return bounds;
}
bool PolygonCollider::checkPoint(Vector2f point) {
    if (!inBounds(point)) return false;

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