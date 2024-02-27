#pragma once
#include "Node.hpp"
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

class Collider : public Node {
    protected:
        ColliderType colliderType;
    public:
        Collider(Node* _parent, Transform _transform, ColliderType _coliderType);
        ~Collider();

        ColliderType getType();
        Vector2f getPosition();
        Vector2f getMin();
        Vector2f getMax();

        virtual bool checkPoint(Vector2f point) = 0;
        virtual Vector2f getBounds() = 0;
        virtual vector<Vector2f> getSideVectors() = 0;
        virtual void getMaxProjection(Vector2f directionVector, float & min, float & max) = 0;

        bool inBounds(Vector2f point);
        bool overlappingBounds(Vector2f otherPos, Vector2f otherBounds);
        bool checkCol(Collider * col);
};