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
        static std::optional<Vector2f> CircleCircleHelper(const Transform& transformA, float radiusA, const Transform& transformB, float radiusB);
        static std::optional<Vector2f> CirclePolygonHelper(const Transform& transformA, float radiusA, const Transform& transformB, vector<Vector2f> pointsB);
        static std::optional<Vector2f> PolygonPolygonHelper(const Transform& transformA, vector<Vector2f> pointsA, const Transform& transformB, vector<Vector2f> pointsB);
        

    public:
        Collider(Node* _parent, Transform _transform, Shape * _shape);
        ~Collider();

        inline Vector2f getPosition() const;
        inline float getRotation() const;

        void setPosition(Vector2f pos);
        void setRotation(float rot);

        void updateBounds();
        AABB getBoundingBox() const;
        Shape* getShape() const;

        bool checkPoint(Vector2f point) const;
        bool checkCollision(Collider * other) const;

        CollisionManifold getCollision(Collider* other) const;
        std::optional<Vector2f> getContactPoint(Collider* other) const;
        
};