#pragma once
#include "Maths.hpp"
#include "Transform.hpp"
#include "AABB.hpp"
#include <array>
#include <vector>
using std::vector;
#include <iostream>
/// @brief Collider Shape: stores the untransformed information about the shape
class Shape {
    public:  
        Shape();

        /// @brief Gets the minimal and maximal values for the dot product between the normal and all of the points on the boundry of the shape
        /// @returns {min, max} 
        virtual std::array<float, 2> getProjection(Transform transform, Vector2f normal) const = 0;
        /// @brief returns the vertices and the centre for circles
        virtual vector<Vector2f> getFeatures(Transform transform) const = 0;
        /// @brief Gers all of the normal vectors to test for the SAT and appends them to the list provided
        virtual size_t getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const = 0;

        //virtual float getMaxProjection(Transform transform, Vector2f normal) const = 0;
        //virtual float getMinProjection(Transform transform, Vector2f normal) const = 0;

        /// @brief Checks if a point is inside the shape
        virtual bool checkPoint(Transform transform, Vector2f point) const = 0;
        /// @brief Gets the axis aligned bounding box
        virtual AABB getBoundingBox(Transform transform) const = 0;
};

class Circle : public Shape {
    private:
        float radius;
    public:
        Circle(float radius);
        
        float getRadius() const;

        vector<Vector2f> getFeatures(Transform transform) const;
        std::array<float, 2> getProjection(Transform transform, Vector2f normal) const;
        size_t getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const;
    
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

        size_t getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const;

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