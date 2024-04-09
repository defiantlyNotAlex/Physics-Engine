#include "headers/PhysicsObject.hpp"

float PhysicsObject::gravity = 200;
float PhysicsObject::drag = 0.001; 
float PhysicsObject::angularDrag = 2;


PhysicsObject::PhysicsObject(Transform _transform, Collider* _collider, float _mass, float _inertia, bool _isStatic) : Node (nullptr, _transform) {
    collider = _collider;
    collider->parent = this;
    collider->transform = _transform;

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
PhysicsObject::CollisionPair PhysicsObject::getCollision(PhysicsObject* A, PhysicsObject* B) {
    CollisionPair cp;
    cp.bodyA = A;
    cp.bodyB = B;
    
    cp = A->collider->getCollision(B->collider);
    if (!cp) {
        return cp;
    }

    for (Vector2f contact : cp.contacts) {
        Vector2f rel_vel = B->getLinearVel(contact) - A->getLinearVel(contact);
        cp.relativeVels.push_back(rel_vel);
    }
    
    return cp;
}
void PhysicsObject::solvePositions(CollisionPair cp) {
    // each is moved according to 1 - total_mass / individual_mass
    float mass_ratio_A;
    float mass_ratio_B;
    if (cp.bodyB->isStatic) {
        if (cp.bodyA->isStatic) return; // should never happen
        mass_ratio_A = 1.f;
        mass_ratio_B = 0.f;
    } else if (cp.bodyA->isStatic) {
        mass_ratio_A = 0.f;
        mass_ratio_B = 1.f;
    } else {
        float total_mass = cp.bodyB->mass + cp.bodyA->mass;
        mass_ratio_A = 1.f - total_mass * cp.bodyA->inv_mass;
        mass_ratio_B = 1.f - total_mass * cp.bodyB->inv_mass;
    }

    cp.bodyA->setPosition(cp.bodyA->getPosition() + cp.normal * cp.depth * mass_ratio_A);
    cp.bodyB->setPosition(cp.bodyB->getPosition() + cp.normal * cp.depth * mass_ratio_B);
}

void PhysicsObject::solveImpulse(CollisionPair cm) {
    for (size_t i = 0; i < cm.contacts.size(); i++) {
        Vector2f contact = cm.contacts[i];

        Vector2f relativeVelocity = cm.relativeVels[i];

        const Vector2f ra = contact - cm.bodyA->getPosition();
        const Vector2f rb = contact - cm.bodyB->getPosition();

        const Vector2f raPerp = Vector2f(-ra.y, ra.x);
        const Vector2f rbPerp = Vector2f(-rb.y, rb.x);

        const float m_A_inv = cm.bodyA->inv_mass;
        const float m_B_inv = cm.bodyB->inv_mass;
        const float I_A_inv = cm.bodyA->inv_inertia;
        const float I_B_inv = cm.bodyB->inv_inertia;

        const float raPerpDotN = Maths::dotProd(raPerp, cm.normal);
        const float rbPerpDotN = Maths::dotProd(rbPerp, cm.normal);

        const float denominator = m_A_inv + m_B_inv + 
        (I_A_inv * raPerpDotN * raPerpDotN) + 
        (I_B_inv * rbPerpDotN * rbPerpDotN);

        float restitution = cm.bodyA->material.elasticity * cm.bodyB->material.elasticity;
        const float j = -(1 + restitution) * Maths::dotProd(relativeVelocity, cm.normal) / denominator;
        Vector2f reactionImpulse = j * cm.normal;

        Vector2f tangent = relativeVelocity - Maths::dotProd(relativeVelocity, cm.normal) * cm.normal;
        const float staticFrictionCoeff = (cm.bodyA->material.staticFriction + cm.bodyB->material.staticFriction)*0.5f;
        const float dynamicFrictionCoeff = (cm.bodyA->material.dynamicFriction + cm.bodyB->material.dynamicFriction) * 0.5f;

        if (Maths::nearlyEqual(tangent, Maths::zero())) {
            return;
        } else {
            tangent = Maths::normalise(tangent);
        }

        const float raPerpDotT = Maths::dotProd(raPerp, tangent);
        const float rbPerpDotT = Maths::dotProd(rbPerp, tangent);

        const float denominatorFriction = m_A_inv + m_B_inv +
            (I_A_inv * raPerpDotT * raPerpDotT) + 
            (I_B_inv * rbPerpDotT * rbPerpDotT);

        const float jt = -Maths::dotProd(relativeVelocity, tangent) / denominatorFriction;
        Vector2f frictionImpulse;
        if (std::abs(jt) <= j * staticFrictionCoeff) {
            frictionImpulse = jt * tangent;
        } else {
            frictionImpulse = -j * tangent * dynamicFrictionCoeff;
        }
        cm.bodyA->applyForce(1, frictionImpulse, contact);
        cm.bodyB->applyForce(1, -frictionImpulse, contact);
    }
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