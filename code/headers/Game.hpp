#pragma once
#include "Node.hpp"
#include "Camera.hpp"
#include "PhysicsObject.hpp"

class Game {
    public:
        Game(sf::RenderWindow& window);
        Node* root;
        Camera* mainCamera;

        void update(float dt);
        void draw();
};