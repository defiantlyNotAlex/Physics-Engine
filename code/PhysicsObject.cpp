#include "headers/PhysicsObject.hpp"

float PhysicsObject::gravity = 200;
float PhysicsObject::drag = 0.001; 
float PhysicsObject::angularDrag = 2;


PhysicsObject::PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass, float _inertia, bool _isStatic) : Node (_parent, _transform) {
    collider = _collider;
    velocity = Maths::zero();
    angularVelocity = 0;

    isStatic = _isStatic;

    mass = _mass;
    inertia = _inertia;
    inv_mass = this->isStatic ? 0 : 1.f / this->mass;
    inv_inertia = this->isStatic ? 0 : 1.f / this->inertia;


    material.elasticity = 0;
    material.dynamicFriction = 0.6;
    material.staticFriction = 0.3;
}
PhysicsObject::~PhysicsObject() {
    delete(collider);
}
Vector2f PhysicsObject::getPosition() {
    return transform.pos;
}
void PhysicsObject::setPosition(Vector2f pos) {
    if (isStatic) return;
    transform.pos = pos;
    collider->setPosition(pos);
}
float PhysicsObject::getRotation() {
    return transform.rot;
}
void PhysicsObject::setRotation(float rot) {
    if (isStatic) return;
    transform.rot = rot;
    collider->setRotation(rot);
}
Vector2f PhysicsObject::getLinearVel(Vector2f point) {
    Vector2f displacement = point - getPosition();
    return velocity + angularVelocity * Maths::rotate90_CW(displacement);
}

void PhysicsObject::step(float dt) {
    applyForce(dt, Maths::down() * gravity * this->mass, transform.pos);

    if (isStatic) {
        velocity = Maths::zero();
        angularVelocity = 0;
        return;
    }
    
    applyForce(dt, -velocity * Maths::magnitude(velocity) * drag, this->getPosition());
    applyTorque(dt, -angularDrag * angularVelocity * angularVelocity);

    auto pos = transform.pos;
    auto rot = transform.rot;

    auto moveTo = transform.pos + velocity * dt;
    auto rotateTo = transform.rot + angularVelocity * dt;

    if (!isStatic) {
        transform.pos = moveTo;
        collider->setPosition(moveTo);    

        transform.rot = rotateTo;
        collider->setRotation(rotateTo);
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

    const float m_this_inv = this->inv_mass;
    const float m_other_inv = other->inv_mass;
    const float I_this_inv = this->inv_inertia;
    const float I_other_inv = other->inv_inertia;

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
    if (isStatic) return;
    velocity += dt * force * inv_mass;
    angularVelocity += dt * Maths::crossProd(forcePos - transform.pos, force) * inv_inertia;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    if (isStatic) return;
    angularVelocity += torque * dt * inv_inertia;
}