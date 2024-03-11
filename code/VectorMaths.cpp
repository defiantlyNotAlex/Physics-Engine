#include "headers/VectorMaths.hpp"

Vector2f VectorMaths::rotate90_CW(Vector2f A) {
    return Vector2f(-A.y, A.x);
}
Vector2f VectorMaths::rotate90_ACW(Vector2f A) {
    return Vector2f(A.y, -A.x);
}
Vector2f VectorMaths::rotate(Vector2f A, float angle) {
    return Vector2f (A.x * std::cos(angle) - A.y * std::sin(angle),
                    A.x * std::sin(angle) + A.y * std::cos(angle));
}
float VectorMaths::magnitude(Vector2f A) {
    return std::sqrt(A.x * A.x + A.y * A.y);
}
float VectorMaths::magnitudeSqr(Vector2f A) {
    return A.x * A.x + A.y * A.y;
}
Vector2f VectorMaths::normalise(Vector2f A) {
    return A * (1.f/magnitude(A));
}
Vector2f VectorMaths::componentProd(Vector2f A, Vector2f B) {
    return Vector2f (A.x * B.x, A.y * B.y);
}
float VectorMaths::dotProd(Vector2f A, Vector2f B) {
    return A.x * B.x + A.y * B.y;
}
float VectorMaths::dotProd(Vector3f A, Vector3f B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}
float VectorMaths::crossProd(Vector2f A, Vector2f B) {
    return A.x * B.y - A.y * B.x;
}
Vector3f VectorMaths::crossProd(Vector3f A, Vector3f B) {
    return Vector3f (A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}
Vector3f VectorMaths::convertTo3D(Vector2f A) {
    return Vector3f(A.x, A.y, 0);
}
Vector2f VectorMaths::convertTo2D(Vector3f A) {
    return Vector2f(A.x, A.y);
}
Vector2f VectorMaths::project(Vector2f A, Vector2f B) {
    return VectorMaths::dotProd(A, B) * B;
}
float VectorMaths::projectScaler(Vector2f A, Vector2f B) {
    return VectorMaths::dotProd(A, B) / magnitude(B);
}
Vector2f VectorMaths::directionVector(float angle) {
    return Vector2f (std::cos(angle), std::sin(angle));
}
Vector2f VectorMaths::clampMagnitude(Vector2f A, float min, float max) {
    float magnitude = VectorMaths::magnitude(A);
    if (magnitude < min) {
        return VectorMaths::normalise(A) * min;
    } else if (magnitude > max) {
        return VectorMaths::normalise(A) * max;
    }
    return A;
}
Vector2i VectorMaths::floorToInt(Vector2f A) {
    return Vector2i(std::floor(A.x), std::floor(A.y));
}
Vector2i VectorMaths::ceilToInt(Vector2f A) {
    return Vector2i(std::ceil(A.x), std::ceil(A.y));
}
Vector2f VectorMaths::toFloat(Vector2i A) {
    return Vector2f(A.x, A.y);
}

bool VectorMaths::nearlyEqual(Vector2f A, Vector2f B) {
    return VectorMaths::magnitude(A - B) < FloatMaths::epsilon;
}
size_t VectorMaths::hash(Vector2i A) {
    return ((size_t)A.x << 32) + (size_t)A.y;
}

string VectorMaths::toString(Vector2f A) {
    return "(" + std::to_string(A.x) + ", " + std::to_string(A.y) + ")";
}
string VectorMaths::toString(Vector3f A) {
    return "(" + std::to_string(A.x) + ", " + std::to_string(A.y)+ ", " + std::to_string(A.z) + ")";
}
string VectorMaths::toString(Vector2i A) {
    return "(" + std::to_string(A.x) + ", " + std::to_string(A.y) + ")";
}

Vector2f VectorMaths::zero() {return Vector2f(0, 0);}
Vector2f VectorMaths::one() {return Vector2f(1, 1);};

Vector2f VectorMaths::left() {return Vector2f(-1, 0);};
Vector2f VectorMaths::right() {return Vector2f(1, 0);};
Vector2f VectorMaths::up() {return Vector2f(0, -1);};
Vector2f VectorMaths::down() {return Vector2f(0, 1);};