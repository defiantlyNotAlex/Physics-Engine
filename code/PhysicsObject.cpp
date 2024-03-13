#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;

float PhysicsObject::gravity = 200;
float PhysicsObject::drag = 0.001; 
float PhysicsObject::angularDrag = 2;

void PhysicsObject::physicsUpdate(float dt) {
    for (PhysicsObject * obj : *objectList) {
        obj->move(dt);
    }
}

PhysicsObject::PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass, float _inertia) : Node (_parent, _transform) {
    objectList->push_back(this);
    collider = _collider;
    velocity = VectorMaths::zero();
    angularVelocity = 0;
    mass = _mass;
    inertia = _inertia;
    material.elasticity = 0;
    material.dynamicFriction = 0.6;
    material.staticFriction = 0.3;
}
PhysicsObject::~PhysicsObject() {
    delete(collider);
    objectList->erase(std::remove(objectList->begin(), objectList->end(), this), objectList->end());
}
Vector2f PhysicsObject::getPosition() const {
    return transform.pos;
}
void PhysicsObject::setPosition(Vector2f pos) {
    if (lockPosition) return;
    //velocity = (pos - transform.pos);
    transform.pos = pos;
    collider->setPosition(pos);
}
float PhysicsObject::getRotation() const {
    return transform.rot;
}
void PhysicsObject::setRotation(float rot) {
    if (lockRotation) return;
    transform.rot = rot;
    collider->setRotation(rot);
}
Vector2f PhysicsObject::getLinearVel(Vector2f point) const {
    Vector2f displacement = point - getPosition();
    return velocity + angularVelocity * VectorMaths::rotate90_CW(displacement);
}

void PhysicsObject::move(float dt) {
    applyForce(dt, VectorMaths::down() * gravity * this->mass, transform.pos);

    if (lockPosition) velocity = VectorMaths::zero();
    if (lockRotation) angularVelocity = 0;

    if (lockPosition && lockRotation) return;

    // air drag and rotational drag
    // drag is proportional to velocity squared
    // rotational drag is proportional to the angular velocity squared
    applyForce(dt, -velocity * VectorMaths::magnitude(velocity) * drag, this->getPosition());
    applyTorque(dt, -angularDrag * angularVelocity * angularVelocity);

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

    vector<PhysicsObject*> others = getAllOverlap();
    for (PhysicsObject* other : others) {
        Collision(dt, other);
    } 
}
void PhysicsObject::Collision(float dt, PhysicsObject* other) {
    CollisionManifold cm = collider->getCollision(other->collider);

    if (!cm.contact.has_value()) {
        std::cout << cm.depth << std::endl;
        std::cout << "no point" << std::endl;
        std::cout << VectorMaths::toString(cm.normal) << std::endl;
        return;
    }
    this->setPosition(this->getPosition() + cm.normal * cm.depth);
    Vector2f contact = cm.contact.value();
    Vector2f relativeVelocity = other->getLinearVel(contact) - this->getLinearVel(contact);

    const Vector2f ra = contact - this->getPosition();
    const Vector2f rb = contact - other->getPosition();

    const Vector2f raPerp = Vector2f(-ra.y, ra.x);
    const Vector2f rbPerp = Vector2f(-rb.y, rb.x);

    const float m_this_inv = this->lockPosition ? 0 : 1.f / this->mass;
    const float m_other_inv = other->lockPosition ? 0 : 1.f / other->mass;
    const float I_this_inv = this->lockRotation ? 0 : 1.f / this->inertia;
    const float I_other_inv = other->lockRotation ? 0 : 1.f / other->inertia;

    const float raPerpDotN = VectorMaths::dotProd(raPerp, cm.normal);
    const float rbPerpDotN = VectorMaths::dotProd(rbPerp, cm.normal);

    const float denominator = m_this_inv + m_other_inv + 
    (I_this_inv * raPerpDotN * raPerpDotN) + 
    (I_other_inv * rbPerpDotN * rbPerpDotN);

    float restitution = this->material.elasticity * other->material.elasticity;
    const float j = -(1 + restitution) * VectorMaths::dotProd(relativeVelocity, cm.normal) / denominator;
    Vector2f reactionImpulse = j * cm.normal;

    this->applyForce(1, -reactionImpulse, contact);
    other->applyForce(1, reactionImpulse, contact);

    Vector2f tangent = relativeVelocity - VectorMaths::dotProd(relativeVelocity, cm.normal) * cm.normal;
    const float staticFrictionCoeff = (this->material.staticFriction + other->material.staticFriction)*0.5f;
    const float dynamicFrictionCoeff = (this->material.dynamicFriction + other->material.dynamicFriction) * 0.5f;

    if (VectorMaths::nearlyEqual(tangent, VectorMaths::zero())) {
        return;
    } else {
        tangent = VectorMaths::normalise(tangent);
    }

    const float raPerpDotT = VectorMaths::dotProd(raPerp, tangent);
    const float rbPerpDotT = VectorMaths::dotProd(rbPerp, tangent);

    const float denominatorFriction = m_this_inv + m_other_inv +
        (I_this_inv * raPerpDotT * raPerpDotT) + 
        (I_other_inv * rbPerpDotT * rbPerpDotT);

    const float jt = -VectorMaths::dotProd(relativeVelocity, tangent) / denominatorFriction;
    Vector2f frictionImpulse;
    if (std::abs(jt) <= j * staticFrictionCoeff) {
        frictionImpulse = jt * tangent;
    } else {
        frictionImpulse = -j * tangent * dynamicFrictionCoeff;
    }
    this->applyForce(1, frictionImpulse, contact);
    other->applyForce(1, -frictionImpulse, contact);

}

void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    velocity += dt * force / mass;
    angularVelocity += dt * VectorMaths::crossProd(forcePos - transform.pos, force) / inertia;
    if (lockPosition) velocity = VectorMaths::zero();
    if (lockRotation) angularVelocity = 0;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    angularVelocity += torque * dt / inertia;
}
/// @brief finds if the PhysicsObject is colliding with anything
/// @return the first object it finds returns nullptr if there are no collisions
PhysicsObject* PhysicsObject::getOverlap() const {
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCollision(other->collider)) {
            return other;
        }
    }
    return nullptr;
}
vector<PhysicsObject*> PhysicsObject::getAllOverlap() const{
    vector<PhysicsObject*> returnArray;
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCollision(other->collider)) {
            returnArray.push_back(other);
        }
    }
    return returnArray;
}
bool PhysicsObject::checkPoint(Vector2f point) const {
    for (PhysicsObject* other : *objectList) {
        if (other != this && other->collider->checkPoint(point)) {
            return true;
        }
    }
    return false;
}
PhysicsObject* PhysicsObject::getObjectAtPoint(Vector2f point) {
    for (PhysicsObject* obj : *objectList) {
        if (obj->collider->checkPoint(point)) {
            return obj;
        }
    }
    return nullptr;
}