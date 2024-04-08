#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;

float PhysicsObject::gravity = 200;
float PhysicsObject::drag = 0.001; 
float PhysicsObject::angularDrag = 2;


PhysicsObject::PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass, float _inertia) : Node (_parent, _transform) {
    objectList->push_back(this);
    collider = _collider;
    velocity = Maths::zero();
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
Vector2f PhysicsObject::getLinearVel(Vector2f point) {
    Vector2f displacement = point - getPosition();
    return velocity + angularVelocity * Maths::rotate90_CW(displacement);
}
void PhysicsObject::update(float dt) {
    applyForce(dt, Maths::down() * gravity * this->mass, transform.pos);
    move(dt);
}

void PhysicsObject::move(float dt) {
    if (lockPosition) velocity = Maths::zero();
    if (lockRotation) angularVelocity = 0;

    if (lockPosition && lockRotation) return;
    applyForce(dt, -velocity * Maths::magnitude(velocity) * drag, this->getPosition());
    applyTorque(dt, -angularDrag * angularVelocity * angularVelocity);

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
        std::cout << Maths::toString(cm.normal) << std::endl;
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

    const float raPerpDotN = Maths::dotProd(raPerp, cm.normal);
    const float rbPerpDotN = Maths::dotProd(rbPerp, cm.normal);

    const float denominator = m_this_inv + m_other_inv + 
    (I_this_inv * raPerpDotN * raPerpDotN) + 
    (I_other_inv * rbPerpDotN * rbPerpDotN);

    float restitution = this->material.elasticity * other->material.elasticity;
    const float j = -(1 + restitution) * Maths::dotProd(relativeVelocity, cm.normal) / denominator;
    Vector2f reactionImpulse = j * cm.normal;

    this->applyForce(1, -reactionImpulse, contact);
    other->applyForce(1, reactionImpulse, contact);

    Vector2f tangent = relativeVelocity - Maths::dotProd(relativeVelocity, cm.normal) * cm.normal;
    const float staticFrictionCoeff = (this->material.staticFriction + other->material.staticFriction)*0.5f;
    const float dynamicFrictionCoeff = (this->material.dynamicFriction + other->material.dynamicFriction) * 0.5f;

    if (Maths::nearlyEqual(tangent, Maths::zero())) {
        return;
    } else {
        tangent = Maths::normalise(tangent);
    }

    const float raPerpDotT = Maths::dotProd(raPerp, tangent);
    const float rbPerpDotT = Maths::dotProd(rbPerp, tangent);

    const float denominatorFriction = m_this_inv + m_other_inv +
        (I_this_inv * raPerpDotT * raPerpDotT) + 
        (I_other_inv * rbPerpDotT * rbPerpDotT);

    const float jt = -Maths::dotProd(relativeVelocity, tangent) / denominatorFriction;
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
    angularVelocity += dt * Maths::crossProd(forcePos - transform.pos, force) / inertia;
    if (lockPosition) velocity = Maths::zero();
    if (lockRotation) angularVelocity = 0;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    angularVelocity += torque * dt / inertia;
}
/// @brief finds if the PhysicsObject is colliding with anything
/// @return the first object it finds returns nullptr if there are no collisions
PhysicsObject* const PhysicsObject::getOverlap() {
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCollision(other->collider)) {
            return other;
        }
    }
    return nullptr;
}
vector<PhysicsObject*> const PhysicsObject::getAllOverlap() {
    vector<PhysicsObject*> returnArray;
    for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCollision(other->collider)) {
            returnArray.push_back(other);
        }
    }
    return returnArray;
}
bool const PhysicsObject::checkPoint(Vector2f point) {
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