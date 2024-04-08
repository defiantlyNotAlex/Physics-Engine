#pragma once
#include "Maths.hpp"
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
        Vector2f edgeVector() const {return end - start;};
        void printEdge() const {std::cout << "start: " << Maths::toString(start) << " end: " << Maths::toString(end) << std::endl;}
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
        Shape::Type getType() const;


        virtual vector<Vector2f> getPoints() const;

        virtual size_t getNormalVectors(Transform transform, vector<Vector2f>& out) const = 0;
        virtual float getMaxProjection(Transform transform, Vector2f normal) const = 0;
        virtual float getMinProjection(Transform transform, Vector2f normal) const = 0;
        virtual vector<Edge> getEdges(Transform transform, Vector2f normal) const = 0;
        virtual bool checkPoint(Transform transform, Vector2f point) const = 0;
        virtual AABB getBoundingBox(Transform transform) const = 0;
};

class Rect : public Shape {
    private:
        Vector2f size;
        vector<Vector2f> points;
    public:
        Rect(Vector2f size);
        
        Vector2f getSize() const;
        vector<Vector2f> getPoints() const;

        size_t getNormalVectors(Transform transform, vector<Vector2f>& out) const;
        float getMaxProjection(Transform transform, Vector2f normal) const;
        float getMinProjection(Transform transform, Vector2f normal) const;
        vector<Edge> getEdges(Transform transform, Vector2f normal) const;
        bool checkPoint(Transform transform, Vector2f point) const;
        AABB getBoundingBox(Transform transform) const;
};

class Circle : public Shape {
    private:
        float radius;
    public:
        Circle(float radius);
        
        float getRadius() const;

        size_t getNormalVectors(Transform transform, vector<Vector2f>& out) const;
        float getMaxProjection(Transform transform, Vector2f normal) const;
        float getMinProjection(Transform transform, Vector2f normal) const;
        vector<Edge> getEdges(Transform transform, Vector2f normal) const;
        bool checkPoint(Transform transform, Vector2f point) const;
        AABB getBoundingBox(Transform transform) const;
};

class Polygon : public Shape {
    private:
        vector<Vector2f> points;
    public:
        Polygon(vector<Vector2f> points);
        
        vector<Vector2f> getPoints() const;

        size_t getNormalVectors(Transform transform, vector<Vector2f>& out) const;
        float getMaxProjection(Transform transform, Vector2f normal) const;
        float getMinProjection(Transform transform, Vector2f normal) const;
        vector<Edge> getEdges(Transform transform, Vector2f normal) const;
        bool checkPoint(Transform transform, Vector2f point) const;
        AABB getBoundingBox(Transform transform) const;
};
