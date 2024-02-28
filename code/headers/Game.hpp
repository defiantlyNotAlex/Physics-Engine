#pragma once
#include "Node.hpp"
#include "Camera.hpp"
#include "PhysicsObject.hpp"

class Game {
    public:
        Node* root;
        Camera* mainCamera;

        void update(float dt);
};