#include "headers/Shape.hpp"

Polygon::Polygon(vector<Vector2f> _points) : Shape(Shape::Type::Polygon) {
    points = _points;
}  
vector<Vector2f> Polygon::getPoints() const {
    return points;
}
size_t Polygon::getNormalVectors(Transform transform, vector<Vector2f>& out) const {
    for (size_t i = 1; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i-1]);
        auto curr = transform.convertLocaltoWorld(points[i]);
        out.push_back(Maths::normalise(Maths::rotate90_ACW(curr - prev)));   
    }
    return points.size();
}
float Polygon::getMaxProjection(Transform transform, Vector2f normal) const {
    float max;
    for (size_t i = 0; i < points.size(); i++) {
        float d = Maths::dotProd(normal, transform.convertLocaltoWorld(points[i]));
        if (i == 0 || d > max) {
            max = d;
        }
    }
    return max;
}
float Polygon::getMinProjection(Transform transform, Vector2f normal) const {
    float min;
    for (size_t i = 0; i < points.size(); i++) {
        float d = Maths::dotProd(normal, transform.convertLocaltoWorld(points[i]));
        if (i == 0 || d < min) {
            min = d;
        }
    }
    return min;
}
vector<Edge> Polygon::getEdges(Transform transform, Vector2f normal) const {
    vector<Edge> returnArray;
    for (size_t i = 0; i < points.size(); i++) {
        returnArray.push_back(Edge(points[i], points[(i+1)%points.size()]));
    }
    return returnArray;
}
bool Polygon::checkPoint(Transform transform, Vector2f point) const {
    for (size_t i = 0; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i]);
        auto curr = transform.convertLocaltoWorld(points[(i+1)%points.size()]);
        Vector2f side = curr - prev;
        Vector2f displacement = point - prev;
        if (Maths::crossProd(displacement, side) > 0) {
            return false;
        }
    }
    return true;
}
AABB Polygon::getBoundingBox(Transform transform) const {
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