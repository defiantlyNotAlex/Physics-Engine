#include "headers/Node.hpp"

Node::Node(Node* _parent, Transform _transform) {
    parent = parent;
    transform = _transform;
}
Node::Node() {
    parent = nullptr;
    transform = Transform({0, 0});
}

void Node::update(float dt) {return;}
void Node::addChild(Node* child) {
    children.push_back(child);
}
void Node::removeChild(Node* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}