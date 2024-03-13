#pragma once
#include "Node.hpp"
#include "Collider.hpp"

class PhysicsObject : public Node {
    private:
        void Collision(float dt, PhysicsObject* other);
    public:
        static vector<PhysicsObject*>* objectList;

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
        vector<Vector2i> chunkList;

        /// @brief updates all physics objects
        /// @param dt 
        static void physicsUpdate(float dt);
        /// @brief attempts to move and rotate
        void move(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        void applyTorque(float dt, float torque);

        Vector2f getPosition() const;
        void setPosition(Vector2f pos);
        float getRotation() const;
        void setRotation(float rot);

        Vector2f getLinearVel(Vector2f point) const;

        PhysicsObject* getOverlap() const;
        vector<PhysicsObject*> getAllOverlap() const;
        bool checkPoint(Vector2f point) const;
        static PhysicsObject* getObjectAtPoint(Vector2f point);
};