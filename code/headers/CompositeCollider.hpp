#pragma once
#include "Collider.hpp"

class CompositeCollider : public Collider {
    private:
        vector<Collider*> colliders;
    public:
        CompositeCollider(Node* _parent, Transform _transform, vector<Collider*> _colliders, vector<Vector2f> _origins);

        vector<Vector2f> colliders;
        vector<Vector2f> origins;
            
        Vector2f getMin();
        Vector2f getMax();

        bool checkPoint(Vector2f point);
        vector<Vector2f> getSideVectors();
        void getMaxProjection(Vector2f directionVector, float & min, float & max);
};