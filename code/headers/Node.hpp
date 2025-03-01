#pragma once
#include "Transform.hpp"
#include <vector>
using std::vector;

class Node {
    friend class World; // allows world to call propagateUpdate
    protected:
        void propagateUpdate(float dt);
    public:
        
        Transform transform;
        vector<Node*> children;
        
        Node* parent;

        Node(Node* parent, Transform _transform);
        Node(Transform _transform) : Node(nullptr, _transform) {};
        Node();
        virtual ~Node();
        
        virtual void update(float dt);
        Node* addChild(Node* component);
        void removeChild(Node* component);

        template <class T>
        T* getChild();
};