#include "headers/Collider.hpp"
#include <iostream>

Collider::Collider(Transform _transform, Shape* _shape) : Node(nullptr, _transform) {
    shape = _shape;
    updateBounds();
}
Collider::Collider(Shape* _shape) : Collider(Transform(), _shape) {};
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

    this->shape->getNormalVectors(this->transform, other->shape->getFeatures(other->transform), normalVectors);
    other->shape->getNormalVectors(other->transform, this->shape->getFeatures(this->transform), normalVectors);

    for (Vector2f normal : normalVectors) {
        std::array<float, 2> thisExtreama = this->shape->getProjection(this->transform, normal);
        std::array<float, 2> otherExtreama = other->shape->getProjection(other->transform, normal);
        
        if (!(thisExtreama[0] < otherExtreama[1] && thisExtreama[1] > otherExtreama[0])) return false;
    }
    return true;
}
optional<Collider::Collision> Collider::getCollision(Collider* other) {
    Collision res;
    if (!boundingBox.checkOverlap(other->boundingBox)) return {};

    vector<Vector2f> normalVectors;

    this->shape->getNormalVectors(this->transform, other->shape->getFeatures(other->transform), normalVectors);
    other->shape->getNormalVectors(other->transform, this->shape->getFeatures(this->transform), normalVectors);

    for (size_t i = 0; i < normalVectors.size(); i++) {
        Vector2f normal = normalVectors[i];

        std::array<float, 2> thisExtreama = this->shape->getProjection(this->transform, normal);
        std::array<float, 2> otherExtreama = other->shape->getProjection(other->transform, normal);

        if (i == 0 || thisExtreama[1] - otherExtreama[0] < res.depth || otherExtreama[1] - thisExtreama[0] < res.depth) {
            if (thisExtreama[1] - otherExtreama[0] <= otherExtreama[1] - thisExtreama[0]) {
                res.normal = -normal;
                res.depth = thisExtreama[1] - otherExtreama[0];
            } else {
                res.normal = normal;
                res.depth = otherExtreama[1] - thisExtreama[0];
            }
        }

        if (!(thisExtreama[0] < otherExtreama[1] && thisExtreama[1] > otherExtreama[0])) return {};
    }
    res.contacts = getContactPoint(other);
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

vector<Vector2f> Collider::getContactPoint(Collider* other) {
    Circle* A_circ = dynamic_cast<Circle*>(this->getShape());
    Circle* B_circ = dynamic_cast<Circle*>(other->getShape());
    if (A_circ != nullptr && B_circ != nullptr) {
        return CircleCircleHelper(this->transform, A_circ->getRadius(), other->transform, B_circ->getRadius());
    }
    Polygon* A_poly = dynamic_cast<Polygon*>(this->getShape());
    Polygon* B_poly = dynamic_cast<Polygon*>(other->getShape());
    if (A_circ != nullptr) {
        return CirclePolygonHelper(this->transform, A_circ->getRadius(), other->transform, B_poly->getPoints());
    }
    if (B_circ != nullptr) {
        return CirclePolygonHelper(other->transform, B_circ->getRadius(), this->transform, A_poly->getPoints());
    }
    return PolygonPolygonHelper(this->transform, A_poly->getPoints(), other->transform, B_poly->getPoints());
}

vector<Vector2f> Collider::CircleCircleHelper(Transform& transformA, float radiusA, Transform& transformB, float radiusB) {
    const Vector2f displacement = transformA.pos - transformB.pos;
    if (Maths::magnitude(displacement) > radiusA + radiusB) {
        return {};
    }
    const Vector2f d = Maths::normalise(displacement);
    return {(transformA.pos + d * radiusA + transformB.pos - d * radiusB) * 0.5f}; 
}
vector<Vector2f> Collider::CirclePolygonHelper(Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB) {
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
vector<Vector2f> Collider::PolygonPolygonHelper(Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB) {
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