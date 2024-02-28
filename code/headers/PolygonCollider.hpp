#pragma once
#include "Collider.hpp"

class PolygonCollider : public Collider {
    private:
        /// @brief vertexes of the rect declared in clockwise order
        vector<Vector2f> points;
    public:
        PolygonCollider(Node* _parent, Transform _transform, vector<Vector2f> _points);
        ~PolygonCollider();

        Vector2f getMin();
        Vector2f getMax();
        vector<Vector2f>& getPoints();

        bool checkPoint(Vector2f point);
        vector<Vector2f> getSideVectors();
        void getMaxProjection(Vector2f directionVector, float & min, float & max);
};