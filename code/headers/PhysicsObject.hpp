#pragma once
#include "Node.hpp"
#include "Colliders.hpp"

class PhysicsObject : public Node {
    private:
        
    public:
        Vector2f velocity;
        float angularVelocity;

        void update(float dt);
};