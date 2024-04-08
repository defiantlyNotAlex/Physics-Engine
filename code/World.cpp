#include "headers/World.hpp"
World::World() {

}
void World::update(float dt) {
    // make it recursively call update so that it doesn't need to be done in the root
    root->update(dt);
}
void World::physicsUpdate(float dt, size_t iterations) {
    // step

    // get collisions

    // solve collisions
}

void World::delObject(PhysicsObject* obj) {
    objectList.erase(std::find(objectList.begin(), objectList.end(), obj), objectList.end());
    delete(obj);
}
void World::newObject(PhysicsObject* obj) {
    objectList.push_back(obj);
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