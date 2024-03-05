#pragma once
#include "VectorUtils.hpp"
#include <iostream>
struct CollisionManifold {
    public:
        Vector2f normal;
        Vector2f contact;
        float depth;
        bool exists;
        inline operator bool() const {return exists;};
};

struct Edge {
    public:
        Vector2f start;
        Vector2f end;
        Vector2f best;
        Edge(Vector2f s, Vector2f e, Vector2f b) {start = s; end = e; best = b;};
        Edge() {};
        Vector2f edgeVector() {return end - start;};
        void printEdge() {std::cout << "start: " << start.x << ", " << start.y << "end: " << end.x << ", " << end.y << "best: " << best.x << ", " << best.y << std::endl;}
};