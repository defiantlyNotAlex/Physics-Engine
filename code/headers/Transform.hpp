#pragma once
#include <SFML/System/Vector2.hpp>
using sf::Vector2f;
#include "VectorUtils.hpp"

class Transform {
    public:
        Transform(Vector2f _position, float _rotation = 0.f);
        Transform();
        Vector2f pos;
        float rot;
        Vector2f convertLocaltoWorld(Vector2f localPos);
        Vector2f convertWorldtoLocal(Vector2f worldPos);
};