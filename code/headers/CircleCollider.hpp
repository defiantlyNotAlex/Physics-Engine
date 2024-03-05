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
        void getMaxProjection(Vector2f directionVector, float & min, float & max);

        size_t getSupportPoints(Vector2f dir, vector<Vector2f>& support);
        Edge getBestEdge(Vector2f normal);
};