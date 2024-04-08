#pragma once
#include "Node.hpp"
#include "Collider.hpp"

class PhysicsObject : public Node {
    private:
        void Collision(float dt, PhysicsObject* other);
    public:

        static float gravity;
        static float drag;
        static float angularDrag;

        PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass = 1, float _inertia = 1000, bool lockPos = false, bool lockRot = false);
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

        bool lockRotation = false;
        bool lockPosition = false;

        Collider* collider;
        vector<Vector2i> chunkList;

        void update(float dt);
        /// @brief attempts to move and rotate
        void step(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        void applyTorque(float dt, float torque);

        Vector2f getPosition();
        void setPosition(Vector2f pos);
        float getRotation();
        void setRotation(float rot);

        Vector2f getLinearVel(Vector2f point);
};