#pragma once
#include <cmath>
#include <cfloat>
#include <algorithm>

class FloatUtils {
    public:
        static float degreeToRad(float angle);
        static float radToDegree(float angle);

        static constexpr float pi = 3.14159265358979323846;
        static constexpr float half_pi = 1.57079632679489661923;
        static constexpr float double_pi = 2 * 3.14159265358979323846;

        static constexpr float epsilon = 1e-8;

};