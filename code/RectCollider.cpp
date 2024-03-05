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

size_t RectCollider::getNormalVectors(vector<Vector2f>& out) {
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

size_t RectCollider::getSupportPoints(Vector2f dir, vector<Vector2f>& support) {
    float max;
    vector<size_t> indexes;
    for (size_t i = 0; i < points.size(); i++) {
        float d = VectorUtils::dotProd(dir, points[i]);
        if (i == 0 || d > max - FloatUtils::epsilon) {
            if (std::abs(d - max) > FloatUtils::epsilon) {
                indexes.clear();
                max = d;
            }
            indexes.push_back(i);
        }
    }
    for (size_t index : indexes) {
        support.push_back(points[index]);
    }
    return indexes.size();
}

Edge RectCollider::getBestEdge(Vector2f normal) {
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

    auto prev_point = transform.convertLocaltoWorld(points[prev_vert]);
    auto next_point = transform.convertLocaltoWorld(points[next_vert]);
    auto max_point = transform.convertLocaltoWorld(points[max_vert]);

    Vector2f l = VectorUtils::normalise(max_point - next_point);
    Vector2f r = VectorUtils::normalise(max_point - prev_point);

    if (std::abs(VectorUtils::dotProd(r, normal)) <= std::abs(VectorUtils::dotProd(l, normal))) {
        return Edge(prev_point, max_point, max_point);
    }
    return Edge(max_point, next_point, max_point);
}