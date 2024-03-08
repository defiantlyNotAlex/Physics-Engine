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
size_t PolygonCollider::getNormalVectors(vector<Vector2f>& out) {
    for (size_t i = 1; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i-1]);
        auto curr = transform.convertLocaltoWorld(points[i]);
        out.push_back(VectorUtils::normalise(VectorUtils::rotate90_ACW(curr - prev)));   
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

Vector2f PolygonCollider::getSupportPoint(Vector2f normal) {
    float max;
    size_t index;
    for (size_t i = 0; i < points.size(); i++) {
        float d = VectorUtils::dotProd(normal, points[i]);
        if (i == 0 || d > max) {
            max = d;
            index = i;
        }
    }
    return points[index];
}
Edge PolygonCollider::getBestEdge(Vector2f normal) {
    float max;
    size_t max_vert;
    for (size_t i = 0; i < points.size(); i++) {
        const float d = VectorUtils::dotProd(transform.convertLocaltoWorld(points[i]), normal);
        if (i == 0 || d > max) {
            max = d;
            max_vert = i;
        }
    }
    size_t next_vert = (max_vert+1)%points.size();
    size_t prev_vert = (max_vert+points.size()-1)%points.size();

    Vector2f l = VectorUtils::normalise(points[max_vert] - points[next_vert]);
    Vector2f r = VectorUtils::normalise(points[max_vert] - points[prev_vert]);

    auto prev_point = transform.convertLocaltoWorld(points[prev_vert]);
    auto next_point = transform.convertLocaltoWorld(points[next_vert]);
    auto max_point = transform.convertLocaltoWorld(points[max_vert]);

    if (std::abs(VectorUtils::dotProd(r, normal)) <= std::abs(VectorUtils::dotProd(l, normal))) {
        return Edge(prev_point, max_point, max_point);
    }
    return Edge(max_point, next_point, max_point);
}