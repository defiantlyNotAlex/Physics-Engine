#pragma once
#include "Node.hpp"
#include "Collision.hpp"
#include "Shapes.hpp"
#include <optional>
#include <vector>
#include "AABB.hpp"
using std::vector;

class Collider : public Node {
    protected:
        AABB boundingBox;
        Shape* shape;
        
        static float pointSegmentDistace(Vector2f point, Vector2f A, Vector2f B, Vector2f& contactPoint);
        static std::optional<Vector2f> CircleCircleHelper(Transform& transformA, float radiusA, Transform& transformB, float radiusB);
        static std::optional<Vector2f> CirclePolygonHelper(Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB);
        static std::optional<Vector2f> PolygonPolygonHelper(Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB);
        

    public:
        Collider(Node* _parent, Transform _transform, Shape * _shape);
        ~Collider();

        inline const Vector2f getPosition();
        inline const float getRotation();

        void setPosition(Vector2f pos);
        void setRotation(float rot);

        void updateBounds();
        AABB getBoundingBox();
        Shape* getShape();

        bool checkPoint(Vector2f point);
        bool checkCollision(Collider * other);

        CollisionManifold getCollision(Collider* other);
        std::optional<Vector2f> getContactPoint(Collider* other);
        
};