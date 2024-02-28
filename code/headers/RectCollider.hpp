#pragma once
#include "Collider.hpp"

class RectCollider : public Collider {
    private:
        /// @brief vertexes of the rect declared in clockwise order
        vector<Vector2f> points;
        Vector2f size;
    public:
        RectCollider(Node* _parent, Transform _transform, Vector2f _size);
        ~RectCollider();

        Vector2f getMin();
        Vector2f getMax();
        Vector2f getSize();

        bool checkPoint(Vector2f point);
        vector<Vector2f> getSideVectors();
        void getMaxProjection(Vector2f directionVector, float & min, float & max);
};