#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;

float PhysicsObject::gravity = 200;
float PhysicsObject::drag = 0.001; 
float PhysicsObject::angularDrag = 2;


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
void PhysicsObject::physicsUpdate(float dt) {
    for (PhysicsObject* obj : *objectList) {
        obj->move(dt);
    }
    vector<CollisionManifold> collisionList;
    for (size_t i = 0; i < objectList->size(); i++) {
        for (size_t j = i + 1; j < objectList->size(); j++) {
            std::optional<CollisionManifold> cm = getCollision((*objectList)[i], (*objectList)[j]);
            if (cm.has_value()) {
                collisionList.push_back(cm.value());
            }
        }
    }
    for (size_t i = 0; i < collisionList.size(); i++) {
        getImpulses(collisionList[i]);    
    }
    for (size_t i = 0; i < collisionList.size(); i++) {
        resolveCollision(collisionList[i]);
    }
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
    return velocity + angularVelocity * VectorMaths::rotate90_CW(displacement);
}
void PhysicsObject::update(float dt) {
}

void PhysicsObject::move(float dt) {
    applyForce(dt, VectorMaths::down() * gravity * this->mass, transform.pos);

    if (lockPosition) velocity = VectorMaths::zero();
    if (lockRotation) angularVelocity = 0;

    if (lockPosition && lockRotation) return;
    applyForce(dt, -velocity * VectorMaths::magnitude(velocity) * drag, this->getPosition());
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
}
void PhysicsObject::resolveCollision(CollisionManifold cm) {
    float ratio = 0;
    
    if (cm.bodyA->lockPosition) ratio = 0.f;
    if (cm.bodyB->lockPosition) ratio = 1.f;
    cm.bodyA->setPosition(cm.bodyA->getPosition() + cm.normal * cm.depth * ratio);
    cm.bodyB->setPosition(cm.bodyB->getPosition() - cm.normal * cm.depth * (1.f - ratio));
}
void PhysicsObject::getImpulses(CollisionManifold cm) {
    if (cm.contactCount == 0) {
        std::cout << cm.depth << std::endl;
        std::cout << "no point" << std::endl;
        std::cout << VectorMaths::toString(cm.normal) << std::endl;
        return;
    }

    for (size_t i = 0; i < cm.contactCount; i++) {
        Vector2f contact = cm.contacts[i];
        Vector2f relativeVelocity = cm.relativeVelocity[i];

        const Vector2f ra = contact - cm.bodyA->getPosition();
        const Vector2f rb = contact - cm.bodyB->getPosition();

        const Vector2f raPerp = Vector2f(-ra.y, ra.x);
        const Vector2f rbPerp = Vector2f(-rb.y, rb.x);

        const float m_this_inv = cm.bodyA->lockPosition ? 0 : 1.f / cm.bodyA->mass;
        const float m_other_inv = cm.bodyB->lockPosition ? 0 : 1.f / cm.bodyB->mass;
        const float I_this_inv = cm.bodyA->lockRotation ? 0 : 1.f / cm.bodyA->inertia;
        const float I_other_inv = cm.bodyB->lockRotation ? 0 : 1.f / cm.bodyB->inertia;

        const float raPerpDotN = VectorMaths::dotProd(raPerp, cm.normal);
        const float rbPerpDotN = VectorMaths::dotProd(rbPerp, cm.normal);

        const float denominator = m_this_inv + m_other_inv + 
        (I_this_inv * raPerpDotN * raPerpDotN) + 
        (I_other_inv * rbPerpDotN * rbPerpDotN);

        float restitution = cm.bodyA->material.elasticity * cm.bodyB->material.elasticity;
        const float j = -(1 + restitution) * VectorMaths::dotProd(relativeVelocity, cm.normal) / denominator;
        Vector2f reactionImpulse = j * cm.normal;

        cm.bodyA->applyForce(1, -reactionImpulse, contact);
        cm.bodyB->applyForce(1, reactionImpulse, contact);

        Vector2f tangent = relativeVelocity - VectorMaths::dotProd(relativeVelocity, cm.normal) * cm.normal;
        const float staticFrictionCoeff = (cm.bodyA->material.staticFriction + cm.bodyB->material.staticFriction)*0.5f;
        const float dynamicFrictionCoeff = (cm.bodyA->material.dynamicFriction + cm.bodyB->material.dynamicFriction) * 0.5f;

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
        cm.bodyA->applyForce(1, frictionImpulse, contact);
        cm.bodyB->applyForce(1, -frictionImpulse, contact);
    }
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
std::optional<PhysicsObject::CollisionManifold> PhysicsObject::getCollision(PhysicsObject* A, PhysicsObject* B) {
    CollisionManifold cm;
    if (!A->collider->getCollision(B->collider, cm.depth, cm.normal)) {
        return {};
    }
    cm.contactCount = A->collider->getContactPoint(cm.contacts, B->collider);
    cm.bodyA = A;
    cm.bodyB = B;
    for (size_t i = 0; i < cm.contactCount; i++) {
        cm.relativeVelocity[i] = -A->getLinearVel(cm.contacts[i]) + B->getLinearVel(cm.contacts[i]);
    }
    
    return cm;
}