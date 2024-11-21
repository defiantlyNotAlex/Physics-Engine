#include "headers/Maths.hpp"

Vector2f Maths::rotate90_CW(Vector2f A) {
    return Vector2f(-A.y, A.x);
}
Vector2f Maths::rotate90_ACW(Vector2f A) {
    return Vector2f(A.y, -A.x);
}
Vector2f Maths::rotate(Vector2f A, float angle) {
    return Vector2f (A.x * std::cos(angle) - A.y * std::sin(angle),
                    A.x * std::sin(angle) + A.y * std::cos(angle));
}
float Maths::magnitude(Vector2f A) {
    return std::sqrt(A.x * A.x + A.y * A.y);
}
float Maths::magnitudeSqr(Vector2f A) {
    return A.x * A.x + A.y * A.y;
}
Vector2f Maths::normalise(Vector2f A) {
    if (A == Vector2f(0.f, 0.f)) {
        return A;
    }
    return A * (1.f/magnitude(A));
}
Vector2f Maths::componentProd(Vector2f A, Vector2f B) {
    return Vector2f (A.x * B.x, A.y * B.y);
}
float Maths::dotProd(Vector2f A, Vector2f B) {
    return A.x * B.x + A.y * B.y;
}
float Maths::dotProd(Vector3f A, Vector3f B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}
float Maths::crossProd(Vector2f A, Vector2f B) {
    return A.x * B.y - A.y * B.x;
}
Vector3f Maths::crossProd(Vector3f A, Vector3f B) {
    return Vector3f 
    (   A.y * B.z - A.z * B.y, 
        A.z * B.x - A.x * B.z, 
        A.x * B.y - A.y * B.x   );
}
Vector3f Maths::convertTo3D(Vector2f A) {
    return Vector3f(A.x, A.y, 0);
}
Vector2f Maths::convertTo2D(Vector3f A) {
    return Vector2f(A.x, A.y);
}
Vector2f Maths::directionVector(float angle) {
    return Vector2f (std::cos(angle), std::sin(angle));
}
Vector2f Maths::clampMagnitude(Vector2f A, float min, float max) {
    float magnitude = Maths::magnitude(A);
    if (magnitude < min) {
        return Maths::normalise(A) * min;
    } else if (magnitude > max) {
        return Maths::normalise(A) * max;
    }
    return A;
}
Vector2i Maths::floorToInt(Vector2f A) {
    return Vector2i(std::floor(A.x), std::floor(A.y));
}
Vector2i Maths::ceilToInt(Vector2f A) {
    return Vector2i(std::ceil(A.x), std::ceil(A.y));
}
Vector2f Maths::toFloat(Vector2i A) {
    return Vector2f(A.x, A.y);
}

bool Maths::nearlyEqual(Vector2f A, Vector2f B) {
    return Maths::magnitude(A - B) < Maths::epsilon;
}

string Maths::toString(Vector2f A) {
    return "(" + std::to_string(A.x) + ", " + std::to_string(A.y) + ")";
}
string Maths::toString(Vector3f A) {
    return "(" + std::to_string(A.x) + ", " + std::to_string(A.y)+ ", " + std::to_string(A.z) + ")";
}
string Maths::toString(Vector2i A) {
    return "(" + std::to_string(A.x) + ", " + std::to_string(A.y) + ")";
}

Vector2f Maths::zero() {return Vector2f(0, 0);}
Vector2f Maths::one() {return Vector2f(1, 1);};

Vector2f Maths::left() {return Vector2f(-1, 0);};
Vector2f Maths::right() {return Vector2f(1, 0);};
Vector2f Maths::up() {return Vector2f(0, -1);};
Vector2f Maths::down() {return Vector2f(0, 1);};

float Maths::degreeToRad(float angle) {
    return (angle * pi / 180.f);
}
float Maths::radToDegree(float angle) {
    return angle * 180.f / pi;
}
bool Maths::nearlyEqual(float a, float b) {
    return std::abs(a - b) < epsilon;
}
