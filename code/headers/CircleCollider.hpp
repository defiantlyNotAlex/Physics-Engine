#pragma once
#include "Collider.hpp"

class CircleCollider : public Collider {
    private:
        float radius;
    public:
        CircleCollider(Node* _parent, Transform _transform, float _radius);
        
        Vector2f getMin();
        Vector2f getMax();
        float getRadius();

        bool checkPoint(Vector2f point);
        vector<Vector2f> getSideVectors();
        void getMaxProjection(Vector2f directionVector, float & min, float & max);
};