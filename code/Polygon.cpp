#include "headers/Shape.hpp"

Polygon::Polygon(vector<Vector2f> _points) {
    points = _points;
    transformedPoints = _points;
}  
const vector<Vector2f>& Polygon::getPoints() const {
    return points;
}
void Polygon::updateTransformedPoints(Transform transform) {
    for (size_t i = 0; i < points.size(); i++) {
        transformedPoints[i] = transform.convertLocaltoWorld(points[i]);
    }
}
const vector<Vector2f>& Polygon::getTransformedPoints() const{
    TIMERSTART();
    return transformedPoints;
    TIMEREND();
}
size_t Polygon::getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const {
    TIMERSTART();
    for (size_t i = 0; i < points.size(); i++) {
        auto prev = transform.convertLocaltoWorld(points[i]);
        auto curr = transform.convertLocaltoWorld(points[(i+1)%points.size()]);
        out.push_back(Maths::normalise(Maths::rotate90_ACW(curr - prev)));   
    }
    return points.size();
    TIMEREND();
}
std::array<float, 2> Polygon::getProjection(Transform transform, Vector2f normal) const {
    TIMERSTART();
    float min;
    float max;
    for (size_t i = 0; i < points.size(); i++) {
        float d = Maths::dotProd(normal, transform.convertLocaltoWorld(points[i]));
        if (i == 0 || d < min) {
            min = d;
        }
        if (i == 0 || d > max) {
            max = d;
        }
    }
    TIMEREND();
    return {min, max};
}
const vector<Vector2f>& Polygon::getFeatures() const {
    return getTransformedPoints();
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