#pragma once
#include "VectorMaths.hpp"
#include "Transform.hpp"
#include "AABB.hpp"
#include <vector>
using std::vector;
#include <iostream>

struct Edge {
    public:
        Vector2f start;
        Vector2f end;
        Edge(Vector2f s, Vector2f e) {start = s; end = e;};
        Edge() {};
        Vector2f const edgeVector() {return end - start;};
        void const printEdge() {std::cout << "start: " << VectorMaths::toString(start) << " end: " << VectorMaths::toString(end) << std::endl;}
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
        Shape::Type const getType();


        virtual vector<Vector2f> getPoints();

        virtual const size_t getNormalVectors(Transform transform, vector<Vector2f>& out) = 0;
        virtual const float getMaxProjection(Transform transform, Vector2f normal) = 0;
        virtual const float getMinProjection(Transform transform, Vector2f normal) = 0;
        virtual const vector<Edge> getEdges(Transform transform, Vector2f normal) = 0;
        virtual const bool checkPoint(Transform transform, Vector2f point) = 0;
        virtual const AABB getBoundingBox(Transform transform) = 0;
};

class Rect : public Shape {
    private:
        Vector2f size;
        vector<Vector2f> points;
    public:
        Rect(Vector2f size);
        
        Vector2f getSize();
        vector<Vector2f> getPoints();

        const size_t getNormalVectors(Transform transform, vector<Vector2f>& out);
        const float getMaxProjection(Transform transform, Vector2f normal);
        const float getMinProjection(Transform transform, Vector2f normal);
        const vector<Edge> getEdges(Transform transform, Vector2f normal);
        const bool checkPoint(Transform transform, Vector2f point);
        const AABB getBoundingBox(Transform transform);
};

class Circle : public Shape {
    private:
        float radius;
    public:
        Circle(float radius);
        
        float getRadius();

        const size_t getNormalVectors(Transform transform, vector<Vector2f>& out);
        const float getMaxProjection(Transform transform, Vector2f normal);
        const float getMinProjection(Transform transform, Vector2f normal);
        const vector<Edge> getEdges(Transform transform, Vector2f normal);
        const bool checkPoint(Transform transform, Vector2f point);
        const AABB getBoundingBox(Transform transform);
};

class Polygon : public Shape {
    private:
        vector<Vector2f> points;
    public:
        Polygon(vector<Vector2f> points);
        
        vector<Vector2f> getPoints();

        const size_t getNormalVectors(Transform transform, vector<Vector2f>& out);
        const float getMaxProjection(Transform transform, Vector2f normal);
        const float getMinProjection(Transform transform, Vector2f normal);
        const vector<Edge> getEdges(Transform transform, Vector2f normal);
        const bool checkPoint(Transform transform, Vector2f point);
        const AABB getBoundingBox(Transform transform);
};
