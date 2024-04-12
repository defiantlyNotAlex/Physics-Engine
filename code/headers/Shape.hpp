#pragma once
#include "Maths.hpp"
#include "Transform.hpp"
#include "AABB.hpp"
#include <array>
#include <vector>
using std::vector;
#include <iostream>

class Shape {
    public:
        
        Shape();

        virtual std::array<float, 2> getProjection(Transform transform, Vector2f normal) const = 0;
        virtual vector<Vector2f> getFeatures(Transform transform) const = 0;
        virtual void getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const = 0;

        virtual float getMaxProjection(Transform transform, Vector2f normal) const = 0;
        virtual float getMinProjection(Transform transform, Vector2f normal) const = 0;

        virtual bool checkPoint(Transform transform, Vector2f point) const = 0;
        virtual AABB getBoundingBox(Transform transform) const = 0;
};

class Circle : public Shape {
    private:
        float radius;
    public:
        Circle(float radius);
        
        float getRadius() const;

        std::array<float, 2> getProjection(Transform transform, Vector2f normal) const;
        vector<Vector2f> getFeatures(Transform transform) const;
        void getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const;

        size_t getNormalVectors(Transform transform, vector<Vector2f>& out) const;
        
        float getMaxProjection(Transform transform, Vector2f normal) const;
        float getMinProjection(Transform transform, Vector2f normal) const;

        bool checkPoint(Transform transform, Vector2f point) const;
        AABB getBoundingBox(Transform transform) const;
};

class Polygon : public Shape {
    protected:
        vector<Vector2f> points;
    public:
        Polygon(vector<Vector2f> points);
        
        const vector<Vector2f>& getPoints() const;
        vector<Vector2f> getTransformedPoints(Transform transform) const;

        std::array<float, 2> getProjection(Transform transform, Vector2f normal) const;
        vector<Vector2f> getFeatures(Transform transform) const;
        void getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const;

        size_t getNormalVectors(Transform transform, vector<Vector2f>& out) const;
        float getMaxProjection(Transform transform, Vector2f normal) const;
        float getMinProjection(Transform transform, Vector2f normal) const;

        bool checkPoint(Transform transform, Vector2f point) const;
        AABB getBoundingBox(Transform transform) const;
};

class Rect : public Polygon {
    private:
        Vector2f size;
    public:
        Rect(Vector2f size);
        
        Vector2f getSize() const;
};