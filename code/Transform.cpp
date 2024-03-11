#include "headers/Transform.hpp"

Transform::Transform(Vector2f _position, float _rotation) {
    pos = _position;
    rot = _rotation;
}
Transform::Transform() : Transform({0, 0}, 0) {};
Vector2f Transform::convertLocaltoWorld(Vector2f localPos) {
    return VectorMaths::rotate(localPos, rot) + pos;
}
Vector2f Transform::convertWorldtoLocal(Vector2f worldPos) { 
    return VectorMaths::rotate(worldPos - pos, -rot);
}