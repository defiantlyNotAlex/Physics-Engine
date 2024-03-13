#pragma once
#include "Node.hpp"
#include "Camera.hpp"
#include "PhysicsObject.hpp"

class World {
    public:
        sf::RenderWindow* window;

        World(sf::RenderWindow* window);
        Node* root;
        Camera* mainCamera;

        void update(float dt);
        void draw();
};