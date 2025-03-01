#pragma once
#include "Maths.hpp"
#include "Transform.hpp"
#include "AABB.hpp"
#include <array>
#include <vector>
#include "debug.hpp"
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
        virtual const vector<Vector2f>& getFeatures() const = 0;
        /// @brief Gers all of the normal vectors to test for the SAT and appends them to the list provided
        virtual size_t getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const = 0;
        virtual void updateTransformedPoints(Transform transform) = 0;

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
        vector<Vector2f> centre;
    public:
        Circle(float radius);
        
        float getRadius() const;
        void updateTransformedPoints(Transform transform);
        const vector<Vector2f>& getFeatures() const;
        std::array<float, 2> getProjection(Transform transform, Vector2f normal) const;
        size_t getNormalVectors(Transform transform, const vector<Vector2f>& otherFeatures, vector<Vector2f>& out) const;
    
        bool checkPoint(Transform transform, Vector2f point) const;
        AABB getBoundingBox(Transform transform) const;
};

class Polygon : public Shape {
    protected:
        vector<Vector2f> points;
        vector<Vector2f> transformedPoints;
    public:
        Polygon(vector<Vector2f> points);
        
        const vector<Vector2f>& getPoints() const;
        const vector<Vector2f>& getTransformedPoints() const;
        void updateTransformedPoints(Transform transform);

        std::array<float, 2> getProjection(Transform transform, Vector2f normal) const;
        const vector<Vector2f>& getFeatures() const;

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