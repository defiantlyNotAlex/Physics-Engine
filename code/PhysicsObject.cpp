#include "headers/PhysicsObject.hpp"

float PhysicsObject::gravity = 100;
float PhysicsObject::drag = 0.01; 
float PhysicsObject::angularDrag = 10;


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


    material.elasticity = 0.33;
    material.dynamicFriction = 0.4;
    material.staticFriction = 0.6;
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
    if (isStatic) return Maths::zero();
    Vector2f displacement = point - getPosition();
    return velocity + angularVelocity * Maths::rotate90_CW(displacement);
}

void PhysicsObject::step(float dt) {
    TIMERSTART();
    if (isStatic) {
        velocity = Maths::zero();
        angularVelocity = 0;
        return;
    }
    applyForce(dt, Maths::down() * gravity * this->mass);
    applyForce(dt, -velocity * Maths::magnitude(velocity) * drag);
    applyTorque(dt, -angularDrag * angularVelocity);

    auto pos = transform.pos;
    auto rot = transform.rot;

    auto moveTo = transform.pos + velocity * dt;
    auto rotateTo = transform.rot + angularVelocity * dt;

    transform.pos = moveTo;
    collider->setPosition(moveTo);    

    transform.rot = rotateTo;
    collider->setRotation(rotateTo);
    TIMEREND();
}
optional<PhysicsObject::CollisionPair> PhysicsObject::getCollision(PhysicsObject* A, PhysicsObject* B) {
    CollisionPair cp;
    cp.bodyA = A;
    cp.bodyB = B;
    
    auto result = A->collider->getCollision(B->collider);
    if (!result) {
        return {};
    }

    cp = *result;

    for (size_t i = 0; i < cp.contact_count; i++) {
        auto contact = cp.contacts[i];
        Vector2f rel_vel = B->getLinearVel(contact) - A->getLinearVel(contact);
        cp.relativeVels[i] = rel_vel;
    }

    return cp;
}
void PhysicsObject::solvePositions(CollisionPair& col) {
    TIMERSTART();
    // each is moved according to 1 - total_mass / individual_mass
    float mass_ratio_A;
    float mass_ratio_B;
    if (col.bodyB->isStatic) {
        if (col.bodyA->isStatic) return; // should never happen
        mass_ratio_A = 1;
        mass_ratio_B = 0.f;
    } else if (col.bodyA->isStatic) {
        mass_ratio_A = 0.f;
        mass_ratio_B = 1;
    } else {
        float total_mass = col.bodyB->mass + col.bodyA->mass;
        mass_ratio_A = 1 - col.bodyA->mass / total_mass;
        mass_ratio_B = 1 - col.bodyB->mass / total_mass;
    }

    col.bodyA->setPosition(col.bodyA->getPosition() + col.normal * col.depth * mass_ratio_A);
    col.bodyB->setPosition(col.bodyB->getPosition() - col.normal * col.depth * mass_ratio_B);
    TIMEREND();
}

void PhysicsObject::solveImpulse(CollisionPair& col) {
    TIMERSTART();
    struct impulse {
        Vector2f force;
        Vector2f location;
        impulse(Vector2f f, Vector2f l) {
            force = f;
            location = l;
        }
    };
    vector<impulse> impulses;

    for (size_t i = 0; i < col.contact_count; i++) {
        Vector2f contact = col.contacts[i];
        Vector2f relativeVelocity = col.relativeVels[i];

        const Vector2f ra = contact - col.bodyA->getPosition();
        const Vector2f rb = contact - col.bodyB->getPosition();

        const Vector2f raPerp = Vector2f(-ra.y, ra.x);
        const Vector2f rbPerp = Vector2f(-rb.y, rb.x);

        const float m_A_inv = col.bodyA->inv_mass;
        const float m_B_inv = col.bodyB->inv_mass;
        const float I_A_inv = col.bodyA->inv_inertia;
        const float I_B_inv = col.bodyB->inv_inertia;

        const float raPerpDotN = Maths::dotProd(raPerp, col.normal);
        const float rbPerpDotN = Maths::dotProd(rbPerp, col.normal);

        const float denominator = m_A_inv + m_B_inv + 
        (I_A_inv * raPerpDotN * raPerpDotN) + 
        (I_B_inv * rbPerpDotN * rbPerpDotN);

        float restitution = col.bodyA->material.elasticity * col.bodyB->material.elasticity;
        const float j = -(1 + restitution) * Maths::dotProd(relativeVelocity, col.normal) / denominator;
        Vector2f reactionImpulse = j * col.normal;

        impulses.push_back(impulse(reactionImpulse, contact));

        Vector2f tangent = relativeVelocity - Maths::dotProd(relativeVelocity, col.normal) * col.normal;
        const float staticFrictionCoeff = (col.bodyA->material.staticFriction + col.bodyB->material.staticFriction)*0.5f;
        const float dynamicFrictionCoeff = (col.bodyA->material.dynamicFriction + col.bodyB->material.dynamicFriction) * 0.5f;
        
        
        if (Maths::nearlyEqual(tangent, Maths::zero())) {
            continue;
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
        impulses.push_back(impulse(-frictionImpulse, contact));
    }
    // apply the impulses
    for (auto impulse : impulses) {
        col.bodyA->applyForce(1, -impulse.force, impulse.location);
        col.bodyB->applyForce(1, impulse.force, impulse.location);
    }
    TIMEREND();
}

void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    if (isStatic) return;
    velocity += dt * force * inv_mass;
    angularVelocity += dt * Maths::crossProd(forcePos - transform.pos, force) * inv_inertia;
}
void PhysicsObject::applyForce(float dt, Vector2f force) {
    if (isStatic) return;
    velocity += dt * force * inv_mass;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    if (isStatic) return;
    angularVelocity += torque * dt * inv_inertia;
}