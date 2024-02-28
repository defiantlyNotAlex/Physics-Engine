#pragma once
#include "Node.hpp"
#include "Colliders.hpp"

class PhysicsObject : public Node {
    public:
        static vector<PhysicsObject*>* objectList;

        PhysicsObject(Node* _parent, Transform _transform, Collider* _collider);
        ~PhysicsObject();

        static float gravity;
        static float drag;

        Vector2f velocity;
        float angularVelocity;

        Collider* collider;

        void update(float dt);
        /// @brief attempts to move and rotate
        void move(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        bool const checkOverlap();
        bool const checkPoint(Vector2f point);
        bool const rayCast(Vector2f pos, Vector2f step, float maxRaycastDistance = 10);
        bool const rayCastGetPos(Vector2f s_pos, Vector2f step, Vector2f & out, float maxRaycastDistance = 10);
};