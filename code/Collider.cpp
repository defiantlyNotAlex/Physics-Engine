#include "headers/Collider.hpp"
#include <iostream>

Collider::Collider(Node* _parent, Transform _transform, ColliderType _colliderType) : Node (_parent, _transform) {
    colliderType = _colliderType;
}
Collider::~Collider() {}

inline ColliderType Collider::getType() {
    return colliderType;
}
inline const Vector2f Collider::getPosition() {
    return transform.pos;
}
inline const float Collider::getRotation() {
    return transform.rot;
}
Vector2f Collider::getMin() {
    return min;
}
Vector2f Collider::getMax() {
    return max;
}

void Collider::setPosition(Vector2f pos) {
    transform.pos = pos;
    updateBounds();
}
void Collider::setRotation(float rot) {
    transform.rot = rot;
    updateBounds();
}
bool Collider::inBounds(Vector2f point) {
    Vector2f thisMin = getMin();
    Vector2f thisMax = getMax();
    return point.x > thisMin.x && point.x < thisMax.x && point.y > thisMin.y && point.y < thisMax.y;
}
bool Collider::overlappingBounds(Collider * col) {
    const Vector2f otherMin = col->getMin();
    const Vector2f otherMax = col->getMax(); 

    const Vector2f thisMin = getMin();
    const Vector2f thisMax = getMax();
    
    return thisMin.x < otherMax.x
        && thisMax.x > otherMin.x
        && thisMin.y < otherMax.y
        && thisMax.y > otherMin.y;
}
bool Collider::checkCol(Collider* other) {
    if (!overlappingBounds(other)) return false;

    vector<Vector2f> dirVectors;
    auto displacement = VectorUtils::normalise(other->getPosition() - getPosition());
    //dirVectors.push_back(displacement);

    this->getNormalVectors(dirVectors);
    other->getNormalVectors(dirVectors);

    for (Vector2f dirVector : dirVectors) {
        float thisMin = FLT_MAX;
        float thisMax = -FLT_MAX;

        float otherMin = FLT_MAX;
        float otherMax = -FLT_MAX;
        
        this->getMaxProjection(dirVector, thisMin, thisMax);
        other->getMaxProjection(dirVector, otherMin, otherMax);
        
        if (!(thisMin < otherMax && thisMax > otherMin)) return false;
    }
    return true;
}

CollisionManifold Collider::getOverlap(Collider* other) {
    CollisionManifold res;
    if (!overlappingBounds(other)) return res;

    vector<Vector2f> normalVectors;
    //auto displacement = VectorUtils::normalise(other->getPosition() - getPosition());
    //normalVectors.push_back(displacement);

    this->getNormalVectors(normalVectors);
    other->getNormalVectors(normalVectors);

    for (size_t i = 0; i < normalVectors.size(); i++) {
        Vector2f d = normalVectors[i];

        float thisMin = FLT_MAX;
        float thisMax = -FLT_MAX;

        float otherMin = FLT_MAX;
        float otherMax = -FLT_MAX;

        this->getMaxProjection(d, thisMin, thisMax);
        other->getMaxProjection(d, otherMin, otherMax);

        if (i == 0 || thisMax - otherMin < res.depth || otherMax - thisMin < res.depth) {
            if (thisMax - otherMin <= otherMax - thisMin) {
                res.normal = -d;
                res.depth = thisMax - otherMin;
            } else {
                res.normal = d;
                res.depth = otherMax - thisMin;
            }
        }
    }

    if (this->colliderType == ColliderType::Circle)  {
        res.contact = this->getSupportPoint(res.normal);
    } else if (other->colliderType == ColliderType::Circle) {
        res.contact = other->getSupportPoint(-res.normal);
    } else {
        res.contact = getContactPoint(other, res.normal);
    }
    
    return res;
}
vector<Vector2f> Collider::clipPoints(Vector2f start, Vector2f end, Vector2f normal, float dot) {
    vector<Vector2f> clippedPoints;
    float startDot = VectorUtils::dotProd(start, normal) - dot;
    float endDot = VectorUtils::dotProd(end, normal) - dot;

    if (startDot >= 0.0) clippedPoints.push_back(start);
    if (endDot >= 0.0) clippedPoints.push_back(end);

    if (startDot * endDot < 0.0) {
        const Vector2f edge = end - start;
        const float t = startDot / (startDot - endDot);

        clippedPoints.push_back(start + (t * edge));
    }
    return clippedPoints;
}
std::optional<Vector2f> Collider::getContactPoint(Collider* other, Vector2f normal) {
    auto edgeThis = this->getBestEdge(-normal);
    auto edgeOther = other->getBestEdge(normal);

    Edge reference;
    Edge incident;
    bool flip = false;

    normal = {0, -1};

    float d_this = std::abs(VectorUtils::dotProd(edgeThis.edgeVector(), normal));
    float d_other = std::abs(VectorUtils::dotProd(edgeOther.edgeVector(), normal));

    

    std::cout << VectorUtils::toString(edgeThis.edgeVector()) << std::endl;
    std::cout << VectorUtils::toString(edgeOther.edgeVector()) << std::endl;
    std::cout << normal.x << " " << normal.y << std::endl;
    std::cout << "d: " << d_this << " " << d_other << std::endl;
    if (std::abs(d_this - d_other) < FloatUtils::epsilon) {
        std::cout << "small" << std::endl;
    }
    
    if (d_this <= d_other) {
        reference = edgeThis;
        incident = edgeOther;
    } else {
        reference = edgeOther;
        incident = edgeThis;
        // flips the incident normal for the final result
        flip = true;
    }

    reference.printEdge();
    incident.printEdge();

    auto referenceEdge = VectorUtils::normalise(reference.edgeVector());

    float referenceStartDot = VectorUtils::dotProd(referenceEdge, reference.start);
    auto clippedPoints = Collider::clipPoints(incident.start, incident.end, referenceEdge, referenceStartDot);
    if (clippedPoints.size() < 2) {
        if (clippedPoints.size() == 0) {
            return {};
        }
        return clippedPoints[0];
    }

    float referenceEndDot = VectorUtils::dotProd(referenceEdge, reference.end);
    clippedPoints = Collider::clipPoints(clippedPoints[0], clippedPoints[1], -referenceEdge, -referenceEndDot);
    if (clippedPoints.size() < 2) {
        if (clippedPoints.size() == 0) {
            return {};
        }
        return clippedPoints[0];
    }
    
    Vector2f referenceNormal = VectorUtils::rotate90_ACW(referenceEdge);
    if (flip) {referenceNormal *= -1.0f;}

    float max = VectorUtils::dotProd(referenceNormal, reference.best);

    std::cout << VectorUtils::toString(clippedPoints[0]) << ", " << VectorUtils::toString(clippedPoints[1]) << std::endl;
    std::cout << max << std::endl;


    std::cout << VectorUtils::dotProd(referenceNormal, clippedPoints.front()) << std::endl;
    std::cout << VectorUtils::dotProd(referenceNormal, clippedPoints.back()) << std::endl;
    if (VectorUtils::dotProd(referenceNormal, clippedPoints.front()) - max < 0.0) {
        clippedPoints.front() = clippedPoints.back();
        clippedPoints.pop_back();
    }
    if (VectorUtils::dotProd(referenceNormal, clippedPoints.back()) - max < 0.0) {
        clippedPoints.pop_back();
    }

    if (clippedPoints.size() == 0) {
        return {};
    }

    Vector2f averagePoint = VectorUtils::zero();
    for (Vector2f cp : clippedPoints) {
        averagePoint += cp;
    }

    return averagePoint / (float)clippedPoints.size();
}