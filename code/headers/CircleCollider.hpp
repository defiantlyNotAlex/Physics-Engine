#pragma once
#include "Collider.hpp"

class CircleCollider : public Collider {
    private:
        float radius;
    public:
        CircleCollider(Node* _parent, Transform _transform, float _radius);
        float getRadius();

        void updateBounds();
        bool checkPoint(Vector2f point);
        size_t getNormalVectors(vector<Vector2f>& out);
        void getMaxProjection(Vector2f normal, float & min, float & max);

        Vector2f getSupportPoint(Vector2f normal);
        Edge getBestEdge(Vector2f normal);
};