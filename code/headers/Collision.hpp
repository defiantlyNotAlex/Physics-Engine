#pragma once
#include "VectorMaths.hpp"
#include "PhysicsObject.hpp"
struct CollisionManifold {
    public:
        PhysicsObject* bodyA;
        PhysicsObject* bodyB;
        Vector2f relativeVelocity;
        Vector2f contacts[2];
        size_t contactCount;
        Vector2f normal;
        float depth;

        //CollisionManifold(PhysicsObject* A, PhysicsObject* B);
};