#pragma once
#include "Transform.hpp"
#include <vector>
using std::vector;

class Node {
    public:
        Transform transform;
        vector<Node*> children;
        
        Node* parent;

        Node(Node* parent, Transform _transform);
        Node();
        ~Node();
        void propagateUpdate(float dt);
        virtual void update(float dt);
        Node* addChild(Node* component);
        void removeChild(Node* component);
};