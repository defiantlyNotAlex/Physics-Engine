#pragma once
#include "Node.hpp"
#include "Colliders.hpp"

class PhysicsObject : public Node {
    private:
        static unordered_map<size_t, vector<PhysicsObject*>>* chunkMap;
        static Vector2f chunkSize;
    public:
        static vector<PhysicsObject*>* objectList;
        inline static Vector2f getChunkSize();
        static vector<PhysicsObject*>* getObjectsInChunk(Vector2i chunk);
        static void removeObjectFromChunk(Vector2i chunk, PhysicsObject* object);
        static void addObjectToChunk(Vector2i chunk, PhysicsObject* object);
        inline static Vector2i convertWorldToChunk(Vector2f world);

        PhysicsObject(Node* _parent, Transform _transform, Collider* _collider, float _mass = 1, float _ineria = 1);
        ~PhysicsObject();

        static float gravity;
        static float drag;
        static float angularDrag;

        Vector2f velocity;
        float angularVelocity;
        float mass;
        float inertia;

        Collider* collider;
        vector<Vector2i> chunkList;

        void update(float dt);
        /// @brief attempts to move and rotate
        void move(float dt);
        void applyForce(float dt, Vector2f force, Vector2f forcePos);
        void applyTorque(float dt, float torque);

        void updateChunks();

        inline const Vector2f getPosition();
        void setPosition(Vector2f pos);
        inline const float getRotation();
        void setRotation(float rot);

        bool const findOverlapPoint();
        bool const checkOverlap();
        bool const checkPoint(Vector2f point);
        bool const rayCast(Vector2f pos, Vector2f step, float maxRaycastDistance = 10);
        bool const rayCastGetPos(Vector2f s_pos, Vector2f step, Vector2f & out, float maxRaycastDistance = 10);
};