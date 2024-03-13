#pragma once
#include <SFML/System/Vector2.hpp>
using sf::Vector2f;
using sf::Vector2i;
#include "VectorMaths.hpp"

class Transform {
    public:
        Transform(Vector2f _position, float _rotation = 0.f);
        Transform();
        Vector2f pos;
        float rot;
        Vector2f convertLocaltoWorld(Vector2f localPos) const;
        Vector2f convertWorldtoLocal(Vector2f worldPos) const;
};