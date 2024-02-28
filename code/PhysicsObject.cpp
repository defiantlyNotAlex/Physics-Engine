#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;
float PhysicsObject::gravity = 10;
float PhysicsObject::drag = 0; 

PhysicsObject::PhysicsObject(Node* _parent, Transform _transform, Collider* _collider) : Node (_parent, _transform) {
    objectList->push_back(this);
    collider = _collider;
    velocity = VectorUtils::zero();
    angularVelocity = 0;
}
PhysicsObject::~PhysicsObject() {
    delete(collider);
    objectList->erase(std::remove(objectList->begin(), objectList->end(), this), objectList->end());
}
void PhysicsObject::update(float dt) {
    applyForce(dt, VectorUtils::down() * gravity, transform.pos);
    move(dt);
}
void PhysicsObject::move(float dt) {
    auto moveTo = transform.pos + velocity * dt;
    auto rotateTo = transform.rot + angularVelocity * dt;

    transform.pos = moveTo;
    transform.rot = rotateTo;

    collider->transform.pos = moveTo;
    collider->transform.rot = rotateTo;
}
void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    velocity += force * dt;
    angularVelocity += VectorUtils::crossProd(forcePos - transform.pos, force) * dt;
}
bool const PhysicsObject::checkOverlap() {
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCol(other->collider)) {
            return true;
        }
    }
    return false;
}
bool const PhysicsObject::checkPoint(Vector2f point) {
    for (PhysicsObject* other : *objectList) {
        if (other != this && other->collider->checkPoint(point)) {
            return true;
        }
    }
    return false;
}
bool const PhysicsObject::rayCast(Vector2f s_pos, Vector2f step, float maxRaycastDistance) {
    for (Vector2f pos = s_pos; VectorUtils::magnitude(pos - s_pos) < maxRaycastDistance; pos += step) {
        if (checkPoint(pos)) {
            return true;
        }
    }
    return false;
}
bool const PhysicsObject::rayCastGetPos(Vector2f s_pos, Vector2f step, Vector2f & out, float maxRaycastDistance) {
    const int binSearchDepth = 4;
    for (Vector2f pos = s_pos; VectorUtils::magnitude(pos - s_pos) < maxRaycastDistance; pos += step) {
        if (checkPoint(pos)) {
            for (int search = 0; search < binSearchDepth; ++search) {
                if (search == 0 || checkPoint(pos)) {
                    step /= 2.f;
                    pos -= step;
                } else {
                    step /= 2.f;
                    pos += step;
                }
            }
            out = pos;
            return true;
        }
    }
    return false;
}