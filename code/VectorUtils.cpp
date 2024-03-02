#include "headers/VectorUtils.hpp"

Vector2f VectorUtils::perp(Vector2f A) {
    return Vector2f(-A.y, A.x);
}
Vector2f VectorUtils::rotate(Vector2f point, float angle) {
    return Vector2f (point.x * std::cos(angle) - point.y * std::sin(angle),
                    point.x * std::sin(angle) + point.y * std::cos(angle));
}
float VectorUtils::magnitude(Vector2f A) {
    return std::sqrt(A.x * A.x + A.y * A.y);
}
float VectorUtils::magnitudeSqr(Vector2f A) {
    return A.x * A.x + A.y * A.y;
}
Vector2f VectorUtils::normalise(Vector2f A) {
    return A * (1.f/magnitude(A));
}
Vector2f VectorUtils::componentProd(Vector2f A, Vector2f B) {
    return Vector2f (A.x * B.x, A.y * B.y);
}
float VectorUtils::dotProd(Vector2f A, Vector2f B) {
    return A.x * B.x + A.y * B.y;
}
float VectorUtils::crossProd(Vector2f A, Vector2f B) {
    return A.x * B.y - A.y * B.x;
}
Vector2f VectorUtils::project(Vector2f A, Vector2f B) {
    return VectorUtils::dotProd(A, B) * B;
}
float VectorUtils::projectScaler(Vector2f A, Vector2f B) {
    return VectorUtils::dotProd(A, B) / magnitude(B);
}
Vector2f VectorUtils::directionVector(float angle) {
    return Vector2f (std::cos(angle), std::sin(angle));
}
Vector2i VectorUtils::floorToInt(Vector2f A) {
    return Vector2i(std::floor(A.x), std::floor(A.y));
}
Vector2i VectorUtils::ceilToInt(Vector2f A) {
    return Vector2i(std::ceil(A.x), std::ceil(A.y));
}
Vector2f VectorUtils::toFloat(Vector2i A) {
    return Vector2f(A.x, A.y);
}

size_t VectorUtils::hash(Vector2i A) {
    return ((size_t)A.x << 32) + (size_t)A.y;
}

Vector2f VectorUtils::zero() {return Vector2f(0, 0);}
Vector2f VectorUtils::one() {return Vector2f(1, 1);};

Vector2f VectorUtils::left() {return Vector2f(-1, 0);};
Vector2f VectorUtils::right() {return Vector2f(1, 0);};
Vector2f VectorUtils::up() {return Vector2f(0, -1);};
Vector2f VectorUtils::down() {return Vector2f(0, 1);};