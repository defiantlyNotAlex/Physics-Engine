#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;

float PhysicsObject::gravity = 10;
float PhysicsObject::drag = 0.01; 
float PhysicsObject::angularDrag = 2;


PhysicsObject::PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass, float _inertia) : Node (_parent, _transform) {
    objectList->push_back(this);
    collider = _collider;
    velocity = VectorUtils::zero();
    angularVelocity = 0;
    mass = _mass;
    inertia = _inertia;
    material.elasticity = 0;
    material.friction = 0.2;
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
    return velocity + angularVelocity * VectorUtils::rotate90_CW(displacement);
}
void PhysicsObject::update(float dt) {
    applyForce(dt, VectorUtils::down() * gravity * this->mass, transform.pos);
    move(dt);
}

void PhysicsObject::move(float dt) {
    if (lockPosition) velocity = VectorUtils::zero();
    if (lockRotation) angularVelocity = 0;
    applyForce(dt, -velocity * VectorUtils::magnitude(velocity) * drag, this->getPosition());
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
    PhysicsObject* other = getOverlap();
    if (other) {
        CollisionManifold cm = collider->getOverlap(other->collider);
        
        if (!cm.contact.has_value()) {
            std::cout << cm.depth << std::endl;
            std::cout << "no point" << std::endl;
            std::cout << VectorUtils::toString(cm.normal) << std::endl;
            //exit(1);
            return;
        }
        this->setPosition(this->getPosition() + cm.normal * cm.depth);
        Vector2f contact = cm.contact.value();

        const Vector2f v_r = other->getLinearVel(contact) - this->getLinearVel(contact);
        const Vector2f displacement = other->getPosition() - this->getPosition();
    
        const Vector3f r_this = VectorUtils::convertTo3D(contact - this->getPosition());
        const Vector3f r_other = VectorUtils::convertTo3D(contact - other->getPosition());
        const Vector3f normal3D = VectorUtils::convertTo3D(cm.normal);

        const float m_this_inv = 1.f / this->mass;
        const float m_other_inv = 1.f / other->mass;

        const float I_this_inv = 1.f / this->inertia;
        const float I_other_inv = 1.f / other->inertia;

        const Vector3f L_this = I_this_inv * VectorUtils::crossProd(VectorUtils::crossProd(r_this, normal3D), r_this);
        const Vector3f L_other = I_other_inv * VectorUtils::crossProd(VectorUtils::crossProd(r_other, normal3D), r_other);

        float elasticicity = 0;
        const float impulseReaction = -(1 + elasticicity) * VectorUtils::dotProd(v_r, cm.normal) 
            / (m_this_inv + m_other_inv + VectorUtils::dotProd(L_this +  L_other, normal3D));

        this->applyForce(1, -impulseReaction * cm.normal, contact);
        other->applyForce(1, impulseReaction * cm.normal, contact);
    }
}
void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    velocity += dt * force / mass;
    angularVelocity += dt * VectorUtils::crossProd(forcePos - transform.pos, force) / inertia;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    angularVelocity += torque * dt / inertia;
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