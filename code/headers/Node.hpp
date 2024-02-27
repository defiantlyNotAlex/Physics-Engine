#pragma once
#include "Transform.hpp"
#include "Tags.hpp"
#include <vector>
using std::vector;

class Node {
    public:
        static Node* root;

        Transform transform;
        vector<Node*> children;
        
        Node* parent;

        Node(Node* parent, Transform _transform);
        Node();
        ~Node();

        virtual void update(float dt);
        void addChild(Node* component);
        void removeChild(Node* component);
};