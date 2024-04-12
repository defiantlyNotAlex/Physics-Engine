#include "headers/Shape.hpp"

Rect::Rect(Vector2f _size) : Polygon(
    {
        Vector2f(-(_size/2.f).x, -(_size/2.f).y),
        Vector2f((_size/2.f).x, -(_size/2.f).y),
        Vector2f((_size/2.f).x, (_size/2.f).y),
        Vector2f(-(_size/2.f).x, (_size/2.f).y),
    }) {
    size = _size;
}  
Vector2f Rect::getSize() const {
    return size;
}