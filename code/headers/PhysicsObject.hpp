#pragma once
#include "Node.hpp"
#include "Collider.hpp"

class PhysicsObject : public Node {
    public:
        struct CollisionManifold {
            public:
                PhysicsObject* bodyA;
                PhysicsObject* bodyB;
                Vector2f relativeVelocity[2];
                Vector2f contacts[2];
                size_t contactCount;
                Vector2f normal;
                float depth;
        };

        static vector<PhysicsObject*>* objectList; // move to world
        static void physicsUpdate(float dt);
        static std::optional<CollisionManifold> getCollision(PhysicsObject* A, PhysicsObject* B);
        static PhysicsObject* getObjectAtPoint(Vector2f point);

        static float gravity;
        static float drag;
        static float angularDrag;

        PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass = 1, float _ineria = 10);
        ~PhysicsObject();

        Vector2f velocity;
        float angularVelocity;

        float mass;
        float inertia;

        struct Material {
            float staticFriction;
            float dynamicFriction;
            float elasticity;
        } material;

        bool lockRotation = false;
        bool lockPosition = false;

        Collider* collider;
        //vector<Vector2i> chunkList;

        void update(float dt);
        /// @brief attempts to move and rotate
        void move(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        void applyTorque(float dt, float torque);

        Vector2f getPosition();
        void setPosition(Vector2f pos);
        float getRotation();
        void setRotation(float rot);

        Vector2f getLinearVel(Vector2f point);
        static void getImpulses(CollisionManifold cm);
        static void resolveCollision(CollisionManifold cm);

        bool const checkPoint(Vector2f point);
};