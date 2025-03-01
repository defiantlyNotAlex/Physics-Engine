#pragma once
#include "Node.hpp"
#include "Camera.hpp"
#include "PhysicsObject.hpp"

class World {
    private:
        //vector<PhysicsObject*> objectList; 
    public:
        vector<PhysicsObject*> objectList;
        vector<AABB>            boxList;
        World();
        Node* root;
        Camera* mainCamera;

        void delObject(PhysicsObject*);
        PhysicsObject* newObject(PhysicsObject*);

        void update(float dt);
        void physicsUpdate(float dt, size_t iterations);
        void draw(sf::RenderWindow& window) const;

        PhysicsObject* getOverlap(PhysicsObject* A) const;
        bool checkPoint(Vector2f point) const;
        PhysicsObject* getObjectAtPoint(Vector2f point) const;
};