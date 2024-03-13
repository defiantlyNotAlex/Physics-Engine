#include "headers/Collider.hpp"
#include <iostream>

Collider::Collider(Node* _parent, Transform _transform, Shape* _shape) : Node (_parent, _transform) {
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
    auto displacement = VectorMaths::normalise(other->getPosition() - getPosition());
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

bool Collider::getCollision(Collider* other, float& depth, Vector2f& normal) {
    if (!boundingBox.checkOverlap(other->boundingBox)) return false;

    vector<Vector2f> normalVectors;
    auto displacement = VectorMaths::normalise(other->getPosition() - getPosition());
    normalVectors.push_back(displacement);

    this->shape->getNormalVectors(this->transform, normalVectors);
    other->shape->getNormalVectors(other->transform, normalVectors);

    for (size_t i = 0; i < normalVectors.size(); i++) {
        Vector2f d = normalVectors[i];

        float thisMin = this->shape->getMinProjection(this->transform, d);
        float thisMax = this->shape->getMaxProjection(this->transform, d);

        float otherMin = other->shape->getMinProjection(other->transform, d);
        float otherMax = other->shape->getMaxProjection(other->transform, d);

        if (!(thisMin < otherMax && thisMax > otherMin)) return false;

        if (i == 0 || thisMax - otherMin < depth || otherMax - thisMin < depth) {
            if (thisMax - otherMin <= otherMax - thisMin) {
                normal = -d;
                depth = thisMax - otherMin;
            } else {
                normal = d;
                depth = otherMax - thisMin;
            }
        }
    }
    return true;
}

float Collider::pointSegmentDistace(Vector2f P, Vector2f A, Vector2f B, Vector2f& contactPoint) {
    Vector2f AB = B - A;
    Vector2f AP = P - A;

    float proj = VectorMaths::dotProd(AP, AB);
    float ABlenSquared = VectorMaths::magnitudeSqr(AB);
    float d = proj / ABlenSquared;

    if (d <= 0.f) {
        contactPoint = A;
    } else if (d >= 1.f) {
        contactPoint = B;
    } else {
        contactPoint = A + AB * d;
    }
    return VectorMaths::magnitudeSqr(P - contactPoint);
}
size_t Collider::getContactPoint(Vector2f* out, Collider* other) {
    if (this->getShape()->getType() == Shape::Type::Circle && this->getShape()->getType() == Shape::Type::Circle) {
        return CircleCircleHelper(out, this->transform, ((Circle*)this->getShape())->getRadius(), other->transform, ((Circle*)other->getShape())->getRadius());
    }
    if (this->shape->getType() == Shape::Type::Circle) {
        return CirclePolygonHelper(out, this->transform, ((Circle*)this->getShape())->getRadius(), other->transform, other->getShape()->getPoints());
    }
    if (other->shape->getType() == Shape::Type::Circle) {
        return CirclePolygonHelper(out, other->transform, ((Circle*)other->getShape())->getRadius(), this->transform, this->getShape()->getPoints());
    }
    return PolygonPolygonHelper(out, this->transform, this->getShape()->getPoints(), other->transform, other->getShape()->getPoints());
}

size_t Collider::CircleCircleHelper(Vector2f* cp, Transform& transformA, float radiusA, Transform& transformB, float radiusB) {
    const Vector2f displacement = transformA.pos - transformB.pos;
    if (VectorMaths::magnitude(displacement) > radiusA + radiusB) {
        cp = nullptr;
        return 0;
    }
    const Vector2f d = VectorMaths::normalise(displacement);
    cp = new Vector2f((transformA.pos + d * radiusA + transformB.pos - d * radiusB) * 0.5f);
    return 1; 
}
size_t Collider::CirclePolygonHelper(Vector2f* out, Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB) {
    float minDistace;
    Vector2f contactPoint;
    for (size_t i = 0; i < pointsB.size(); i++) {
        const Vector2f A = transformB.convertLocaltoWorld(pointsB[i]);
        const Vector2f B = transformB.convertLocaltoWorld(pointsB[(i+1)%pointsB.size()]);

        
        float distSqrd = pointSegmentDistace(transformA.pos, A, B, contactPoint);
        if (i == 0 || distSqrd < minDistace) {
            minDistace = distSqrd;
            out[0] = contactPoint;
        }
    }
    if (minDistace > radiusA * radiusA) {
        out = nullptr;
        return 0;
    }
    return 1;
}
size_t Collider::PolygonPolygonHelper(Vector2f* out, Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB) {
    Vector2f contact1 = VectorMaths::zero();
    Vector2f contact2 = VectorMaths::zero();
    float minDistace = FLT_MAX;
    size_t contactCount = 0;

    for (size_t i = 0; i < pointsA.size(); i++) {
        Vector2f P = transformA.convertLocaltoWorld(pointsA[i]);
        for (size_t j = 0; j < pointsB.size(); j++) {
            const Vector2f A = transformB.convertLocaltoWorld(pointsB[j]);
            const Vector2f B = transformB.convertLocaltoWorld(pointsB[(j+1)%pointsB.size()]);
            Vector2f cp;
            float distSqr = pointSegmentDistace(P, A, B, cp);
            if (FloatMaths::nearlyEqual(distSqr, minDistace)) {
                if (!VectorMaths::nearlyEqual(cp, contact1)) {
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
            if (FloatMaths::nearlyEqual(distSqr, minDistace)) {
                if (!VectorMaths::nearlyEqual(cp, contact1)) {
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
    out[0] = contact1;
    out[1] = contact2;
    return contactCount;

}   