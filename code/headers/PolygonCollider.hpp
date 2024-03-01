#pragma once
#include "Collider.hpp"

class PolygonCollider : public Collider {
    private:
        /// @brief vertexes of the rect declared in clockwise order
        vector<Vector2f> points;
    public:
        PolygonCollider(Node* _parent, Transform _transform, vector<Vector2f> _points);
        ~PolygonCollider();

        vector<Vector2f>& getPoints();
        void updateBounds();

        bool checkPoint(Vector2f point);
        vector<Vector2f> getSideVectors();
        void getMaxProjection(Vector2f directionVector, float & min, float & max);
};