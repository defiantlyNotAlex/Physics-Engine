#pragma once
#include <array>

template <class T>
class OptionalPair {
    public:
        std::array<T, 2> data = {Maths::zero(), Maths::zero()};
        size_t count = 0;

        operator bool() const {
            return count > 0;
        }

        OptionalPair() {};
        OptionalPair(std::initializer_list<T> list) {
            for (auto v = list.begin(); v != list.end(); v++) {
                data[std::distance(list.begin(), v)] = *v;
            }
            count = list.size();
        };

        OptionalPair& operator=(std::initializer_list<T> list) {
            for (auto v = list.begin(); v != list.end(); v++) {
                data[std::distance(list.begin(), v)] = *v;
            }
            count = list.size();
            return *this;
        };

        void push_back(const T& obj) {
            if (count == 2) {
                data[1] = obj;
            }
            data[count] = obj;
            count++;
        }
        Vector2f& operator[](unsigned i) {return data[i];};
        unsigned size() const { return count; };

        auto begin() const { return data.begin(); };
        auto end() const { return data.begin() + count; };
};