#include "headers/Collider.hpp"
#include <iostream>

Collider::Collider(Transform _transform, Shape* _shape) : Node(nullptr, _transform) {
    shape = _shape;
    updateBounds();
}
Collider::~Collider() {}

inline const Vector2f Collider::getPosition() {
    return transform.pos;
}
inline const float Collider::getRotation() {
    return transform.rot;
}
AABB Collider::getBoundingBox() {
    return boundingBox;
}
Shape* Collider::getShape() {
    return shape;
}
void Collider::setPosition(Vector2f pos) {
    transform.pos = pos;
    updateBounds();
}
void Collider::setRotation(float rot) {
    transform.rot = rot;
    updateBounds();
}
bool Collider::checkPoint(Vector2f point) {
    return shape->checkPoint(transform, point);
}
void Collider::updateBounds() {
    boundingBox = shape->getBoundingBox(transform);
}
bool Collider::checkCollision(Collider* other) {
    if (!boundingBox.checkOverlap(other->boundingBox)) return false;

    vector<Vector2f> normalVectors;
    auto displacement = Maths::normalise(other->getPosition() - getPosition());
    normalVectors.push_back(displacement);

    this->shape->getNormalVectors(this->transform, normalVectors);
    other->shape->getNormalVectors(other->transform, normalVectors);

    for (Vector2f normal : normalVectors) {
        float thisMin = this->shape->getMinProjection(this->transform, normal);
        float thisMax = this->shape->getMaxProjection(this->transform, normal);

        float otherMin = other->shape->getMinProjection(other->transform, normal);
        float otherMax = other->shape->getMaxProjection(other->transform, normal);

        
        if (!(thisMin < otherMax && thisMax > otherMin)) return false;
    }
    return true;
}

Collider::Collision Collider::getCollision(Collider* other) {
    Collision res;
    res.exists = false;

    if (!boundingBox.checkOverlap(other->boundingBox)) return res;

    vector<Vector2f> normalVectors;
    auto displacement = Maths::normalise(other->getPosition() - getPosition());
    normalVectors.push_back(displacement);

    this->shape->getNormalVectors(this->transform, normalVectors);
    other->shape->getNormalVectors(other->transform, normalVectors);

    for (size_t i = 0; i < normalVectors.size(); i++) {
        Vector2f normal = normalVectors[i];

        float thisMin = this->shape->getMinProjection(this->transform, normal);
        float thisMax = this->shape->getMaxProjection(this->transform, normal);

        float otherMin = other->shape->getMinProjection(other->transform, normal);
        float otherMax = other->shape->getMaxProjection(other->transform, normal);

        if (i == 0 || thisMax - otherMin < res.depth || otherMax - thisMin < res.depth) {
            if (thisMax - otherMin <= otherMax - thisMin) {
                res.normal = -normal;
                res.depth = thisMax - otherMin;
            } else {
                res.normal = normal;
                res.depth = otherMax - thisMin;
            }
        }

        if (!(thisMin < otherMax && thisMax > otherMin)) return res;
    }
    res.contacts = getContactPoint(other);
    res.exists = true;
    return res;
}

float Collider::pointSegmentDistace(Vector2f P, Vector2f A, Vector2f B, Vector2f& contactPoint) {
    Vector2f AB = B - A;
    Vector2f AP = P - A;

    float proj = Maths::dotProd(AP, AB);
    float ABlenSquared = Maths::magnitudeSqr(AB);
    float d = proj / ABlenSquared;

    if (d <= 0.f) {
        contactPoint = A;
    } else if (d >= 1.f) {
        contactPoint = B;
    } else {
        contactPoint = A + AB * d;
    }
    return Maths::magnitudeSqr(P - contactPoint);
}
OptionalPair<Vector2f> Collider::getContactPoint(Collider* other) {
    if (this->getShape()->getType() == Shape::Type::Circle && this->getShape()->getType() == Shape::Type::Circle) {
        return CircleCircleHelper(this->transform, ((Circle*)this->getShape())->getRadius(), other->transform, ((Circle*)other->getShape())->getRadius());
    }
    if (this->shape->getType() == Shape::Type::Circle) {
        return CirclePolygonHelper(this->transform, ((Circle*)this->getShape())->getRadius(), other->transform, other->getShape()->getPoints());
    }
    if (other->shape->getType() == Shape::Type::Circle) {
        return CirclePolygonHelper(other->transform, ((Circle*)other->getShape())->getRadius(), this->transform, this->getShape()->getPoints());
    }
    return PolygonPolygonHelper(this->transform, this->getShape()->getPoints(), other->transform, other->getShape()->getPoints());
}

OptionalPair<Vector2f> Collider::CircleCircleHelper(Transform& transformA, float radiusA, Transform& transformB, float radiusB) {
    const Vector2f displacement = transformA.pos - transformB.pos;
    if (Maths::magnitude(displacement) > radiusA + radiusB) {
        //return {};
    }
    const Vector2f d = Maths::normalise(displacement);
    return {(transformA.pos + d * radiusA + transformB.pos - d * radiusB) * 0.5f}; 
}
OptionalPair<Vector2f> Collider::CirclePolygonHelper(Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB) {
    float minDistace;
    Vector2f cp;
    for (size_t i = 0; i < pointsB.size(); i++) {
        const Vector2f A = transformB.convertLocaltoWorld(pointsB[i]);
        const Vector2f B = transformB.convertLocaltoWorld(pointsB[(i+1)%pointsB.size()]);

        Vector2f contactPoint;
        float distSqrd = pointSegmentDistace(transformA.pos, A, B, contactPoint);
        if (i == 0 || distSqrd < minDistace) {
            minDistace = distSqrd;
            cp = contactPoint;
        }
    }
    if (minDistace > radiusA * radiusA) {
        return {};
    }
    return {cp};
}
OptionalPair<Vector2f> Collider::PolygonPolygonHelper(Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB) {
    Vector2f contact1 = Maths::zero();
    Vector2f contact2 = Maths::zero();
    float minDistace = FLT_MAX;
    size_t contactCount = 0;

    for (size_t i = 0; i < pointsA.size(); i++) {
        Vector2f P = transformA.convertLocaltoWorld(pointsA[i]);
        for (size_t j = 0; j < pointsB.size(); j++) {
            const Vector2f A = transformB.convertLocaltoWorld(pointsB[j]);
            const Vector2f B = transformB.convertLocaltoWorld(pointsB[(j+1)%pointsB.size()]);
            Vector2f cp;
            float distSqr = pointSegmentDistace(P, A, B, cp);
            if (Maths::nearlyEqual(distSqr, minDistace)) {
                if (!Maths::nearlyEqual(cp, contact1)) {
                    contact2 = cp;
                    contactCount = 2;
                }
            } else if (distSqr < minDistace) {
                minDistace = distSqr;
                contact1 = cp;
                contactCount = 1;
            }
        }
    }
    for (size_t i = 0; i < pointsB.size(); i++) {
        Vector2f P = transformB.convertLocaltoWorld(pointsB[i]);
        for (size_t j = 0; j < pointsA.size(); j++) {
            const Vector2f A = transformA.convertLocaltoWorld(pointsA[j]);
            const Vector2f B = transformA.convertLocaltoWorld(pointsA[(j+1)%pointsA.size()]);
            Vector2f cp;
            float distSqr = pointSegmentDistace(P, A, B, cp);
            if (Maths::nearlyEqual(distSqr, minDistace)) {
                if (!Maths::nearlyEqual(cp, contact1)) {
                    contact2 = cp;
                    contactCount = 2;
                }
            } else if (distSqr < minDistace) {
                minDistace = distSqr;
                contact1 = cp;
                contactCount = 1;
            } 
        }
    }
    if (contactCount == 0) return {};
    if (contactCount == 1) return {contact1};
    return {contact1, contact2};
}   