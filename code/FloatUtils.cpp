#include "headers/FloatUtils.hpp"

float FloatUtils::degreeToRad(float angle) {
    return (angle * pi / 180.f);
}
float FloatUtils::radToDegree(float angle) {
    return angle * 180.f / pi;
}