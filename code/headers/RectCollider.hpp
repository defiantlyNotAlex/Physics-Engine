#pragma once
#include "Collider.hpp"

class RectCollider : public Collider {
    private:
        /// @brief vertexes of the rect declared in clockwise order
        vector<Vector2f> points;
        Vector2f size;
    public:
        RectCollider(Transform _transform, Vector2f _size);
        ~RectCollider();

        Vector2f getBounds();
        Vector2f getSize();
        vector<Vector2f> getPoints();
        bool checkPoint(Vector2f point);
        bool checkCol(Collider * col);
        bool overlapRect(RectCollider* r_col);
};