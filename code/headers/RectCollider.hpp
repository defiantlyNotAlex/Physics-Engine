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

        void updateBounds();
        Vector2f getSize();

        bool checkPoint(Vector2f point);
        size_t getEdgeVectors(vector<Vector2f>& out);
        void getMaxProjection(Vector2f directionVector, float & min, float & max);
};