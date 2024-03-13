#pragma once
#include "Node.hpp"
#include "Shapes.hpp"
#include <optional>
#include <vector>
#include "AABB.hpp"
using std::vector;

class Collider : public Node {
    protected:
        AABB boundingBox;
        Shape* shape;

        void updateBounds();
        
        static float pointSegmentDistace(Vector2f point, Vector2f A, Vector2f B, Vector2f& contactPoint);
        static size_t CircleCircleHelper(Vector2f* out, Transform& transformA, float radiusA, Transform& transformB, float radiusB);
        static size_t CirclePolygonHelper(Vector2f* out, Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB);
        static size_t PolygonPolygonHelper(Vector2f* out, Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB);
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

        bool getCollision(Collider* other, float& depth, Vector2f& normal);
        size_t getContactPoint(Vector2f* out, Collider* other);
};