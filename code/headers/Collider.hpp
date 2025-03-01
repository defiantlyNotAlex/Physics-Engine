#pragma once
#include "Node.hpp"
#include "Shapes.hpp"
#include "AABB.hpp"
#include <optional>
using std::optional;
#include <vector>
using std::vector;
#include <array>
using std::array;
#include "debug.hpp"


class Collider : public Node {
    protected:
        AABB boundingBox;
        Shape* shape;
        
        static float pointSegmentDistace(Vector2f point, Vector2f A, Vector2f B, Vector2f& contactPoint);
        static size_t CircleCircleHelper(Transform& transformA, float radiusA, Transform& transformB, float radiusB, array<Vector2f, 2>& out);
        static size_t CirclePolygonHelper(Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB, array<Vector2f, 2>& out);
        static size_t PolygonPolygonHelper(Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB, array<Vector2f, 2>& out);
    public:
        Collider(Transform _transform, Shape * _shape);
        Collider(Shape* _shape);
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


        struct Collision {
            public:
                array<Vector2f, 2> contacts;
                size_t contact_count;
                Vector2f normal;
                float depth;
        };

        optional<Collision> getCollision(Collider* other);
        size_t getContactPoint(Collider* other, array<Vector2f, 2>& out);
};