#include "headers/FloatMaths.hpp"

float FloatMaths::degreeToRad(float angle) {
    return (angle * pi / 180.f);
}
float FloatMaths::radToDegree(float angle) {
    return angle * 180.f / pi;
}
bool FloatMaths::nearlyEqual(float a, float b) {
    return std::abs(a - b) < epsilon;
}
