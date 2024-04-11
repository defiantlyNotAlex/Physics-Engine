#include "headers/Node.hpp"

Node::Node(Node* _parent, Transform _transform) {
    parent = _parent;
    transform = _transform;
}
Node::Node() {
    parent = nullptr;
    transform = Transform({0, 0});
}
Node::~Node() {
    for (Node* child : children) {
        delete(child);
    }
}
void Node::update(float dt) {
    return;
}
void Node::propagateUpdate(float dt) {
    update(dt);
    for (Node* obj : children) {
        obj->propagateUpdate(dt);
    }
}

Node* Node::addChild(Node* child) {
    children.push_back(child);
    child->parent = this;
    return child;
}
void Node::removeChild(Node* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

/// @brief finds the first instance of a specific type and returns it
/// @tparam T must be a class dirived from Node
/// @return returns the reference to the child it has found or nullptr if none are found
template <class T>
T* Node::getChild() {
    for (Node* child : children) {
        T* return_val = dynamic_cast<T*>(child);
        if (return_val != nullptr) {
            return return_val;
        }
    }
    return nullptr;
}