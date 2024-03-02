#pragma once
#include "Node.hpp"
#include "Colliders.hpp"

class PhysicsObject : public Node {
    private:

    public:
        static vector<PhysicsObject*>* objectList;

        static float gravity;
        static float drag;
        static float angularDrag;

        PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass = 1, float _ineria = 1);
        ~PhysicsObject();

        Vector2f velocity;
        float angularVelocity;
        float mass;
        float inertia;
        bool lockRotation = false;
        bool lockPosition = false;

        Collider* collider;
        vector<Vector2i> chunkList;

        void update(float dt);
        /// @brief attempts to move and rotate
        void move(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        void applyTorque(float dt, float torque);

        void updateChunks();

        Vector2f getPosition();
        void setPosition(Vector2f pos);
        float getRotation();
        void setRotation(float rot);

        Vector2f const findOverlapPoint(PhysicsObject* other);
        PhysicsObject* const getOverlap();
        vector<PhysicsObject*> const getAllOverlap();
        size_t const getAllOverlapNoAlloc(vector<PhysicsObject*>& out);
        bool const checkPoint(Vector2f point);
        bool const rayCast(Vector2f pos, Vector2f step, float maxRaycastDistance = 10);
        bool const rayCastGetPos(Vector2f s_pos, Vector2f step, Vector2f & out, float maxRaycastDistance = 10);
};