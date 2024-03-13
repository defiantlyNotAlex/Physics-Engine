#include "headers/Shape.hpp" 
Rect::Rect(Vector2f _size) : Shape(Shape::Type::Rect) {
    size = _size;
    const Vector2f halfSize = size/2.f;
    points = {
        Vector2f(-halfSize.x, -halfSize.y),
        Vector2f(halfSize.x, -halfSize.y),
        Vector2f(halfSize.x, halfSize.y),
        Vector2f(-halfSize.x, halfSize.y),
    };
}  
Vector2f Rect::getSize() {
    return size;
}
vector<Vector2f> Rect::getPoints() {
    return points;
}
size_t Rect::getNormalVectors(Transform transform, vector<Vector2f>& out) const {
    out.push_back(VectorMaths::directionVector(transform.rot));
    out.push_back(VectorMaths::directionVector(transform.rot + FloatMaths::half_pi));
    return 2;
}
float Rect::getMaxProjection(Transform transform, Vector2f normal) const {
    float max;
    for (size_t i = 0; i < points.size(); i++) {
        float d = VectorMaths::dotProd(normal, transform.convertLocaltoWorld(points[i]));
        if (i == 0 || d > max) {
            max = d;
        }
    }
    return max;
}
float Rect::getMinProjection(Transform transform, Vector2f normal) const {
    float min;
    for (size_t i = 0; i < points.size(); i++) {
        float d = VectorMaths::dotProd(normal, transform.convertLocaltoWorld(points[i]));
        if (i == 0 || d < min) {
            min = d;
        }
    }
    return min;
}
vector<Edge> Rect::getEdges(Transform transform, Vector2f normal) const {
    vector<Edge> returnArray;
    for (size_t i = 0; i < points.size(); i++) {
        returnArray.push_back(Edge(points[i], points[(i+1)%points.size()]));
    }
    return returnArray;
}
bool Rect::checkPoint(Transform transform, Vector2f point) const {
    for (size_t i = 0; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i]);
        auto curr = transform.convertLocaltoWorld(points[(i+1)%points.size()]);
        Vector2f side = curr - prev;
        Vector2f displacement = point - prev;
        if (VectorMaths::crossProd(displacement, side) > 0) {
            return false;
        }
    }
    return true;
}
AABB Rect::getBoundingBox(Transform transform) const {
    AABB newBoundingBox;
    for (size_t i = 0; i < points.size(); i++) {
        Vector2f d = transform.convertLocaltoWorld(points[i]);
        if (i == 0) {
            newBoundingBox.max = d;
            newBoundingBox.min = d;
            continue;
        }
        newBoundingBox.min.x = std::min(d.x, newBoundingBox.min.x);
        newBoundingBox.min.y = std::min(d.y, newBoundingBox.min.y);

        newBoundingBox.max.x = std::max(d.x, newBoundingBox.max.x);
        newBoundingBox.max.y = std::max(d.y, newBoundingBox.max.y);
    }
    return newBoundingBox;
}