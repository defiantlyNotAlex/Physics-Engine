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


class Collider : public Node {
    protected:
        AABB boundingBox;
        Shape* shape;
        
        static float pointSegmentDistace(Vector2f point, Vector2f A, Vector2f B, Vector2f& contactPoint);
        static vector<Vector2f> CircleCircleHelper(Transform& transformA, float radiusA, Transform& transformB, float radiusB);
        static vector<Vector2f> CirclePolygonHelper(Transform& transformA, float radiusA, Transform& transformB, vector<Vector2f> pointsB);
        static vector<Vector2f> PolygonPolygonHelper(Transform& transformA, vector<Vector2f> pointsA, Transform& transformB, vector<Vector2f> pointsB);
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
                vector<Vector2f> contacts;
                Vector2f normal;
                float depth;
        };

        optional<Collision> getCollision(Collider* other);
        vector<Vector2f> getContactPoint(Collider* other);
};