#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
using sf::Vector2f;
using sf::Vector2i;
using sf::Vector3f;
#include <string>
using std::string;
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_map>
using std::unordered_map;

class Maths {
    public:
        static Vector2f zero();
        static Vector2f one();

        static Vector2f left();
        static Vector2f right();
        static Vector2f up();
        static Vector2f down();

        static string toString(Vector2f A);
        static string toString(Vector3f A);
        static string toString(Vector2i A);
        /// @brief rotates the vector by 90 degrees in the clockwise direction
        static Vector2f rotate90_CW(Vector2f A);
        /// @brief rotates the vector by 90 degrees in an anticlockwise direction
        static Vector2f rotate90_ACW(Vector2f A);
        /// @brief rotates a vector about the origin by an arbritrary angle 
        static Vector2f rotate(Vector2f A, float angle);
        /// @brief componentwise product of A and B
        static Vector2f componentProd(Vector2f A, Vector2f B);
        /// @brief dot product of A and B
        static float dotProd(Vector2f A, Vector2f B);
        static float dotProd(Vector3f A, Vector3f B);
        static float crossProd(Vector2f A, Vector2f B);
        static Vector3f crossProd(Vector3f A, Vector3f B);
        static Vector2f convertTo2D(Vector3f A);
        static Vector3f convertTo3D(Vector2f A);
        static float magnitude(Vector2f A);
        static float magnitudeSqr(Vector2f A);
        static Vector2f normalise(Vector2f A);
        /// @brief Projects A onto B
        static Vector2f project(Vector2f A, Vector2f B);
        /// @brief Projects A onto B returning a scaler
        static float projectScaler(Vector2f A, Vector2f B);
        static Vector2f directionVector(float angle);
        static Vector2f clampMagnitude(Vector2f value, float min, float max);
        
        static Vector2i floorToInt(Vector2f A);
        static Vector2i ceilToInt(Vector2f A);
        static Vector2f toFloat(Vector2i A);

        static bool nearlyEqual(float a, float b);
        static bool nearlyEqual(Vector2f A, Vector2f B);

        static size_t hash(Vector2i A);

        static float degreeToRad(float angle);
        static float radToDegree(float angle);

        static constexpr float pi = 3.14159265358979323846;
        static constexpr float half_pi = 1.57079632679489661923;
        static constexpr float double_pi = 2 * 3.14159265358979323846;

        static constexpr float epsilon = 1e-5;        
        static constexpr float one_plus_epsilon = epsilon + 1.f;
};