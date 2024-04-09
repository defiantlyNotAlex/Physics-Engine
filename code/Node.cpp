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