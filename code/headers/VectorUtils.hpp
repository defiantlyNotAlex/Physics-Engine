#pragma once
#include <SFML/System/Vector2.hpp>
using sf::Vector2f;
using sf::Vector2i;
#include "FloatUtils.hpp"
#include <cmath>
#include <cfloat>
#include <algorithm>

class VectorUtils {
    public:
        static Vector2f zero();
        static Vector2f one();

        static Vector2f left();
        static Vector2f right();
        static Vector2f up();
        static Vector2f down();

        static Vector2f rotate(Vector2f point, float angle);
        /// @brief componentwise product of A and B
        static Vector2f componentProd(Vector2f A, Vector2f B);
        /// @brief dot product of A and B
        static float dotProd(Vector2f A, Vector2f B);
        static float magnitude(Vector2f A);
        static float magnitudeSqr(Vector2f A);
        static Vector2f normalise(Vector2f A);
        /// @brief Projects A onto B
        static Vector2f project(Vector2f A, Vector2f B);
        /// @brief Projects A onto B returning a scaler
        static float projectScaler(Vector2f A, Vector2f B);
        static Vector2f directionVector(float angle);
        static Vector2f clampMagnitude(Vector2f value, float min, float max);
        static float crossProd(Vector2f A, Vector2f B);
        static Vector2i floorToInt(Vector2f A);
        static Vector2i ceilToInt(Vector2f A);
        static Vector2f toFloat(Vector2i A);
};