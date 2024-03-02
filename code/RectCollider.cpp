#include "headers/RectCollider.hpp"

RectCollider::RectCollider(Node* _parent, Transform _transform, Vector2f _size) : Collider(_parent, _transform, ColliderType::Rect) {
    size = _size;
    Vector2f halfSize = size/2.f;
    points.push_back(Vector2f (-halfSize.x, -halfSize.y));
    points.push_back(Vector2f (halfSize.x, -halfSize.y));
    points.push_back(Vector2f (halfSize.x, halfSize.y));
    points.push_back(Vector2f (-halfSize.x, halfSize.y));
    updateBounds();
}

Vector2f RectCollider::getSize() {
    return size;
}

void RectCollider::updateBounds() {
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

bool RectCollider::checkPoint(Vector2f point) {
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

size_t RectCollider::getEdgeVectors(vector<Vector2f>& out) {
    out.push_back(VectorUtils::directionVector(transform.rot));
    out.push_back(VectorUtils::directionVector(transform.rot + FloatUtils::half_pi));
    return 2;
}
void RectCollider::getMaxProjection(Vector2f directionVector, float & min, float & max) {
    for (auto point : points) {
        auto t_point = transform.convertLocaltoWorld(point);
        auto projection = VectorUtils::projectScaler(t_point, directionVector);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}