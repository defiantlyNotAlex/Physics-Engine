#pragma once
#include "Transform.hpp"
#include <vector>
using std::vector;

enum class ColliderType{
    None,
    Point,
    Rect,
    Circle,
    Polygon,
    Line,
};

class Collider {
    protected:
        ColliderType colliderType;
    public:
        Transform transform;

        Collider(Transform _transform, ColliderType _coliderType);
        ~Collider();

        ColliderType getType();
        Vector2f getPosition();
        Vector2f getMin();
        Vector2f getMax();

        virtual bool checkPoint(Vector2f point) = 0;
        virtual bool checkCol(Collider * col) = 0;
        virtual Vector2f getBounds() = 0;

        bool inBounds(Vector2f point);
        bool overlappingBounds(Vector2f otherPos, Vector2f otherBounds);
};