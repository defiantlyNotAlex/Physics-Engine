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


namespace Maths {
    Vector2f zero();
    Vector2f one();

    Vector2f left();
    Vector2f right();
    Vector2f up();
    Vector2f down();

    string toString(Vector2f A);
    string toString(Vector3f A);
    string toString(Vector2i A);

    Vector2f rotate90_CW(Vector2f A);
    Vector2f rotate90_ACW(Vector2f A);
    Vector2f rotate(Vector2f A, float angle);

    
    float dotProd(Vector2f A, Vector2f B);
    float crossProd(Vector2f A, Vector2f B);
    float magnitude(Vector2f A);
    float magnitudeSqr(Vector2f A);
    Vector2f componentProd(Vector2f A, Vector2f B);
    Vector2f normalise(Vector2f A);
    Vector2f directionVector(float angle);
    Vector2f clampMagnitude(Vector2f value, float min, float max);

    Vector2f convertTo2D(Vector3f A);
    Vector3f convertTo3D(Vector2f A);

    float dotProd(Vector3f A, Vector3f B);
    Vector3f crossProd(Vector3f A, Vector3f B);
    
    

    Vector2i floorToInt(Vector2f A);
    Vector2i ceilToInt(Vector2f A);
    Vector2f toFloat(Vector2i A);

    bool nearlyEqual(float a, float b);
    bool nearlyEqual(Vector2f A, Vector2f B);

    float degreeToRad(float angle);
    float radToDegree(float angle);

    constexpr float pi = 3.14159265358979323846;
    constexpr float half_pi = 1.57079632679489661923;
    constexpr float double_pi = 2 * 3.14159265358979323846;

    constexpr float epsilon = 1e-4;        
    constexpr float one_plus_epsilon = epsilon + 1.f;
};