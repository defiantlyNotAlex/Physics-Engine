#pragma once
#include "Node.hpp"
#include "Collider.hpp"

class PhysicsObject : public Node {
    private:

    public:
        static float gravity;
        static float drag;
        static float angularDrag;

        PhysicsObject(Transform _transform, Collider* _collider, float _mass = 1, float _inertia = 333, bool _isStatic = false);
        ~PhysicsObject();

        Vector2f velocity;
        float angularVelocity;

        float mass;
        float inertia;

        float inv_mass;
        float inv_inertia;

        struct Material {
            float staticFriction;
            float dynamicFriction;
            float elasticity;
        } material;

        bool isStatic = false;

        Collider* collider;

        /// @brief attempts to move and rotate
        void step(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        void applyForce(float dt, Vector2f force);
        void applyTorque(float dt, float torque);

        Vector2f getPosition();
        void setPosition(Vector2f pos);
        float getRotation();
        void setRotation(float rot);

        struct CollisionPair {
            public:
                PhysicsObject* bodyA;
                PhysicsObject* bodyB;
                
                Vector2f normal;
                float depth;
                array<Vector2f, 2> contacts;
                array<Vector2f, 2> relativeVels;
                size_t contact_count;

                CollisionPair& operator=(Collider::Collision cm) {
                    normal = cm.normal;
                    depth = cm.depth;
                    contacts = cm.contacts;
                    contact_count = cm.contact_count;
                    return *this;
                };
        };

        Vector2f getLinearVel(Vector2f point);
        static optional<CollisionPair> getCollision(PhysicsObject* A, PhysicsObject* B);
        static void solveImpulse(CollisionPair& cm);
        static void solvePositions(CollisionPair& cp);
};