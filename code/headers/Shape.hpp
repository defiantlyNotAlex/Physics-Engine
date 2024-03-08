#pragma once
#include "VectorUtils.hpp"
#include "Transform.hpp"
#include <vector>
using std::vector;
#include <iostream>

struct Box {
    Box();
    Box(Vector2f a, Vector2f b) {min = a, max = b;};
    Vector2f min;
    Vector2f max;
    static bool OverlapBox(Box A, Box B);
    bool const checkPoint(Vector2f point);
};

struct Edge {
    public:
        Vector2f start;
        Vector2f end;
        Vector2f best;
        Edge(Vector2f s, Vector2f e, Vector2f b) {start = s; end = e; best = b;};
        Edge() {};
        Vector2f const edgeVector() {return end - start;};
        void const printEdge() {std::cout << "start: " << VectorUtils::toString(start) << " end: " << VectorUtils::toString(end) << " best: " << VectorUtils::toString(best) << std::endl;}
};

class Shape {
    public:
        enum class Type {
            None,
            Rect,
            Circle,
            Polygon,
        } type;
        
        Shape(Shape::Type _type);
        Shape::Type const inline getType();

        virtual const size_t getNormalVectors(Transform transform, vector<Vector2f>& out) = 0;
        virtual const float getMaxProjection(Transform transform, Vector2f normal) = 0;
        virtual const Edge getBestEdge(Transform transform, Vector2f normal) = 0;
        virtual const bool checkPoint(Transform transform, Vector2f point) = 0;
        virtual const Box getBoundingBox(Transform transform) = 0;
};

