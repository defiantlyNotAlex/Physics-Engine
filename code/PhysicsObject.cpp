#include "headers/PhysicsObject.hpp"

vector<PhysicsObject*>* PhysicsObject::objectList = new vector<PhysicsObject*>;
unordered_map<size_t, vector<PhysicsObject*>>* PhysicsObject::chunkMap = new unordered_map<size_t, vector<PhysicsObject*>>;
Vector2f PhysicsObject::chunkSize = Vector2f(100, 100);

float PhysicsObject::gravity = 10;
float PhysicsObject::drag = 0; 
float PhysicsObject::angularDrag = 1;

inline Vector2f PhysicsObject::getChunkSize() {
    return chunkSize;
}
inline Vector2i PhysicsObject::convertWorldToChunk(Vector2f world) {
    return VectorUtils::floorToInt({world.x / chunkSize.x, world.y / chunkSize.y});
}
vector<PhysicsObject*>* PhysicsObject::getObjectsInChunk(Vector2i chunk) {
    if (chunkMap->find(VectorUtils::hash(chunk)) != chunkMap->end()) {
        return &chunkMap->at(VectorUtils::hash(chunk));
    }
    return new vector<PhysicsObject*> ({});
}
void PhysicsObject::removeObjectFromChunk(Vector2i chunk, PhysicsObject* object) {
    if (chunkMap->find(VectorUtils::hash(chunk)) != chunkMap->end()) {
        vector<PhysicsObject*>* list = &chunkMap->at(VectorUtils::hash(chunk));
        list->erase(std::remove(list->begin(), list->end(), object), list->end());

        if (list->size() == 0) {
            //chunkMap->erase(VectorUtils::hash(chunk));
        }
    }
}
void PhysicsObject::addObjectToChunk(Vector2i chunk, PhysicsObject* object) {
    if (chunkMap->find(VectorUtils::hash(chunk)) != chunkMap->end()) {
        vector<PhysicsObject*>* list = &chunkMap->at(VectorUtils::hash(chunk));
        if (list) {
            list->push_back(object);
            return;
        }
    }
    chunkMap->insert({VectorUtils::hash(chunk), {object}});
}


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
inline const Vector2f PhysicsObject::getPosition() {
    return transform.pos;
}
void PhysicsObject::setPosition(Vector2f pos) {
    transform.pos = pos;
    collider->setPosition(pos);
    updateChunks();
}
inline const float PhysicsObject::getRotation() {
    return transform.rot;
}
void PhysicsObject::setRotation(float rot) {
    transform.rot = rot;
    collider->setRotation(rot);
    updateChunks();
}
void PhysicsObject::update(float dt) {
    applyForce(dt, VectorUtils::down() * gravity, transform.convertLocaltoWorld(VectorUtils::left()));
    move(dt);
    checkOverlap();
}

void PhysicsObject::updateChunks() {
    vector<Vector2i> currChunks;
    for (int x = collider->getMin().x; x <= collider->getMax().x; x++) {
        for (int y = collider->getMin().y; y <= collider->getMax().y; y++) {
            currChunks.push_back({x, y});
        }
    }
    for (Vector2i chunk : chunkList) {
        if (std::find(currChunks.begin(), currChunks.end(), chunk) == currChunks.end()) {
            PhysicsObject::removeObjectFromChunk(chunk, this);
        }
    }
    for (Vector2i chunk : currChunks) {
        if (std::find(chunkList.begin(), chunkList.end(), chunk) == chunkList.end()) {
            PhysicsObject::addObjectToChunk(chunk, this);
        }
    }
    chunkList == currChunks;
}

void PhysicsObject::move(float dt) {
    velocity -= drag * velocity * VectorUtils::magnitude(velocity) * dt;
    angularVelocity -= angularDrag * angularVelocity * dt;

    auto moveTo = transform.pos + velocity * dt;
    auto rotateTo = transform.rot + angularVelocity * dt;

    //transform.pos = moveTo;
    transform.rot = rotateTo;

    //collider->setPosition(moveTo);
    collider->setRotation(rotateTo);
    updateChunks();
}
void PhysicsObject::applyForce(float dt, Vector2f force, Vector2f forcePos) {
    velocity += force / mass * dt;
    angularVelocity += VectorUtils::crossProd(forcePos - transform.pos, force) / inertia * dt;
}
void PhysicsObject::applyTorque(float dt, float torque) {
    angularVelocity += torque * dt;
}
bool const PhysicsObject::checkOverlap() {
    for (Vector2i chunk : chunkList) {
        for (PhysicsObject* other : *getObjectsInChunk(chunk)) {
            if (other != this && this->collider->checkCol(other->collider)) {
                return true;
            }
        }
    }
    /*for (PhysicsObject* other : *objectList) {
        if (other != this && this->collider->checkCol(other->collider)) {
            return true;
        }
    }*/
    
    return false;
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