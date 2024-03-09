#pragma once
#include "Node.hpp"
#include "Colliders.hpp"
#include "Material.hpp"

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

        Material material;

        bool lockRotation = false;
        bool lockPosition = false;

        Collider* collider;
        vector<Vector2i> chunkList;

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

        PhysicsObject* const getOverlap();
        vector<PhysicsObject*> const getAllOverlap();
        bool const checkPoint(Vector2f point);
        static PhysicsObject* getObjectAtPoint(Vector2f point);
};