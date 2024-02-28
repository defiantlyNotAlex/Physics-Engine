#pragma once
#include "Collider.hpp"

class CompositeCollider : public Collider {
    private:
        vector<Collider*> colliders;
    public:
        CompositeCollider(Node* _parent, Transform _transform, vector<Collider*> _colliders);

            
        Vector2f const getMin();
        Vector2f const getMax();

        bool const checkPoint(Vector2f point);
        vector<Vector2f> const getSideVectors();
        void const getMaxProjection(Vector2f directionVector, float & min, float & max);
};