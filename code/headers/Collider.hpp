#pragma once
#include "Node.hpp"
#include "Collision.hpp"
#include <optional>
#include <vector>
using std::vector;

enum class ColliderType{
    None,
    Rect,
    Circle,
    Polygon,
};

class Collider : public Node {
    protected:
        ColliderType colliderType;
        Vector2f min = VectorUtils::zero();
        Vector2f max = VectorUtils::zero();

        // potentially move to vector utils??
        static vector<Vector2f> clipPoints(Vector2f start, Vector2f end, Vector2f normal, float dot);

    public:
        Collider(Node* _parent, Transform _transform, ColliderType _coliderType);
        ~Collider();

        inline ColliderType getType();
        inline const Vector2f getPosition();
        inline const float getRotation();

        void setPosition(Vector2f pos);
        void setRotation(float rot);

        Vector2f getMin();
        Vector2f getMax();

        virtual void updateBounds() = 0;

        virtual bool checkPoint(Vector2f point) = 0;
        virtual size_t getSupportPoints(Vector2f dir, vector<Vector2f>& support) = 0;

        virtual size_t getNormalVectors(vector<Vector2f>& out) = 0;
        virtual void getMaxProjection(Vector2f directionVector, float & min, float & max) = 0;
        virtual Edge getBestEdge(Vector2f normal) = 0;

        bool inBounds(Vector2f point);
        bool overlappingBounds(Collider * other);
        bool checkCol(Collider * other);

        CollisionManifold getOverlap(Collider* other);
        std::optional<Vector2f> getContactPoint(Collider* other, Vector2f dir);
        
};