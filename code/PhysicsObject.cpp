#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;

float PhysicsObject::gravity = 10;
float PhysicsObject::drag = 0; 
float PhysicsObject::angularDrag = 1;


PhysicsObject::PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass, float _inertia) : Node (_parent, _transform) {
    objectList->push_back(this);
    collider = _collider;
    velocity = VectorUtils::zero();
    angularVelocity = 0;
    mass = _mass;
    inertia = _inertia;
}
PhysicsObject::~PhysicsObject() {
    delete(collider);
    objectList->erase(std::remove(objectList->begin(), objectList->end(), this), objectList->end());
}
inline const Vector2f PhysicsObject::getPosition() {
    return transform.pos;
}
void PhysicsObject::setPosition(Vector2f pos) {
    transform.pos = pos;
    collider->setPosition(pos);
}
inline const float PhysicsObject::getRotation() {
    return transform.rot;
}
void PhysicsObject::setRotation(float rot) {
    transform.rot = rot;
    collider->setRotation(rot);
}
void PhysicsObject::update(float dt) {
    applyForce(dt, VectorUtils::down() * gravity, transform.convertLocaltoWorld(VectorUtils::left()));
    move(dt);
    checkOverlap();
}

void PhysicsObject::move(float dt) {
    velocity -= drag * velocity * VectorUtils::magnitude(velocity) * dt;
    angularVelocity -= angularDrag * angularVelocity * dt;

    auto moveTo = transform.pos + velocity * dt;
    auto rotateTo = transform.rot + angularVelocity * dt;

    //transform.pos = moveTo;
    transform.rot = rotateTo;

    //collider->setPosition(moveTo);
    collider->setRotation(rotateTo);
}
void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    velocity += force / mass * dt;
    angularVelocity += VectorUtils::crossProd(forcePos - transform.pos, force) / inertia * dt;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    angularVelocity += torque * dt;
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