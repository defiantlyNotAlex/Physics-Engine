#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;

float PhysicsObject::gravity = 0;
float PhysicsObject::drag = 1; 
float PhysicsObject::angularDrag = 0.1;


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
Vector2f PhysicsObject::getPosition() {
    return transform.pos;
}
void PhysicsObject::setPosition(Vector2f pos) {
    if (lockPosition) return;
    //velocity = (pos - transform.pos);
    transform.pos = pos;
    collider->setPosition(pos);
}
float PhysicsObject::getRotation() {
    return transform.rot;
}
void PhysicsObject::setRotation(float rot) {
    if (lockRotation) return;
    transform.rot = rot;
    collider->setRotation(rot);
}
void PhysicsObject::update(float dt) {
    applyForce(dt, VectorUtils::down() * gravity, transform.pos);
    move(dt);
}

void PhysicsObject::move(float dt) {
    velocity -= drag * velocity * VectorUtils::magnitude(velocity) * dt;
    angularVelocity -= angularDrag * angularVelocity * dt;

    auto pos = transform.pos;
    auto rot = transform.rot;

    auto moveTo = transform.pos + velocity * dt;
    auto rotateTo = transform.rot + angularVelocity * dt;
    if (!lockPosition) {
        transform.pos = moveTo;
        collider->setPosition(moveTo);
    }
    if (!lockRotation) {
        transform.rot = rotateTo;
        collider->setRotation(rotateTo);
    }
    if (getOverlap()) {
        //transform.rot = rot;
        //transform.pos = pos;
        
    }
}
void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    velocity += force / mass * dt;
    angularVelocity += VectorUtils::crossProd(forcePos - transform.pos, force) / inertia * dt;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    angularVelocity += torque * dt;
}
/// @brief finds if the PhysicsObject is colliding with anything
/// @return the first object it finds returns nullptr if there are no collisions
PhysicsObject* const PhysicsObject::getOverlap() {
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCol(other->collider)) {
            return other;
        }
    }
    return nullptr;
}
/// @brief returns a list of all PhysicsObjects which overlap
/// @return returns the array of PhysicsObjects which are colliding
vector<PhysicsObject*> const PhysicsObject::getAllOverlap() {
    vector<PhysicsObject*> returnArray;
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCol(other->collider)) {
            returnArray.push_back(other);
        }
    }
    return returnArray;
}
/// @brief returns a list of all PhysicsObjects which overlap without memery allocation
/// @param out the output parameter
/// @return returns the number of PhysicsObjects which are colliding
size_t const PhysicsObject::getAllOverlapNoAlloc(vector<PhysicsObject*>& out) {
    out.clear();
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCol(other->collider)) {
            out.push_back(other);
        }
    }
    return out.size();
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