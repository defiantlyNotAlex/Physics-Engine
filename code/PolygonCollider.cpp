#include "headers/PolygonCollider.hpp"

PolygonCollider::PolygonCollider(Node* _parent, Transform _transform, vector<Vector2f> _points) : Collider (_parent, _transform, ColliderType::Polygon) {
    points = _points;
}
PolygonCollider::~PolygonCollider() {}

vector<Vector2f>& PolygonCollider::getPoints() {
    return points;
}

void PolygonCollider::updateBounds() {
    min = transform.convertLocaltoWorld(points[0]);
    max = transform.convertLocaltoWorld(points[0]);
    for (Vector2f point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        min.x = std::min(min.x, t_point.x);
        min.y = std::min(min.y, t_point.y);

        max.x = std::max(max.x, t_point.x);
        max.y = std::max(max.y, t_point.y);
    }
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
size_t PolygonCollider::getEdgeVectors(vector<Vector2f>& out) {
    for (size_t i = 1; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i-1]);
        auto curr = transform.convertLocaltoWorld(points[i]);
        out.push_back(VectorUtils::normalise(curr - prev));   
    }
    return points.size();
}
void PolygonCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    for (auto point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        auto projection = VectorUtils::projectScaler(t_point, directionVector);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}