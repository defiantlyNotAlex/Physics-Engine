#pragma once
#include "Node.hpp"
#include "Camera.hpp"
#include "PhysicsObject.hpp"

// todo implement quad tree
// implement physicsUpdate
class World {
    public:
        World(sf::RenderWindow& window);
        Node* root;
        Camera* mainCamera;

        void update(float dt);
        void draw();
};