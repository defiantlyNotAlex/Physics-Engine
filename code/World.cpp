#include "headers/World.hpp"
World::World() {
    root = new Node(nullptr, Transform());
}

void World::update(float dt) {
    root->propagateUpdate(dt);
}
void World::physicsUpdate(float dt, size_t iterations) {
    dt = dt / (float)iterations;
    for (size_t it = 0; it < iterations; it++) {
        for (PhysicsObject* obj : objectList) {
            obj->step(dt);
        }

        vector<PhysicsObject::CollisionPair> collisions;
        
        for (size_t i = 0; i < objectList.size(); i++) {
            for (size_t j = i + 1; j < objectList.size(); j++) {
                PhysicsObject* bodyA = objectList[i];
                PhysicsObject* bodyB = objectList[j];
                if (bodyA->isStatic && bodyB->isStatic) continue;
                
                auto col = PhysicsObject::getCollision(bodyA, bodyB);
                if (col) {
                    collisions.push_back(*col);
                }
            }
        }
        for (auto col : collisions) {
            PhysicsObject::solveImpulse(col);
        }
        for (auto col : collisions) {
            PhysicsObject::solvePositions(col);
        }
    }
}

void World::delObject(PhysicsObject* obj) {
    objectList.erase(std::find(objectList.begin(), objectList.end(), obj), objectList.end());
    delete(obj);
}
PhysicsObject* World::newObject(PhysicsObject* obj) {
    objectList.push_back(obj);
    return obj;
}

/// @brief finds if the PhysicsObject is colliding with anything
/// @return the first object it finds returns nullptr if there are no collisions
PhysicsObject* World::getOverlap(PhysicsObject* A) const {
    for (PhysicsObject* other : objectList) {
        if (other != A && A->collider->checkCollision(other->collider)) {
            return other;
        }
    }
    return nullptr;
}
bool World::checkPoint(Vector2f point) const {
    for (PhysicsObject* obj : objectList) {
        if (obj->collider->checkPoint(point)) {
            return true;
        }
    }
    return false;
}
PhysicsObject* World::getObjectAtPoint(Vector2f point) const {
    for (PhysicsObject* obj : objectList) {
        if (obj->collider->checkPoint(point)) {
            return obj;
        }
    }
    return nullptr;
}