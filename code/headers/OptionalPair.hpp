#pragma once
#include <array>

template <class T>
struct OptionalPair {
    public:
        std::array<T, 2> data;
        size_t count;

        operator bool() const {
            return count > 0;
        }

        OptionalPair& operator=(std::initializer_list<Vector2f> list) {
            for (auto v = list.begin(); v != list.end(); v++) {
                data[std::distance(list.begin(), v)] = *v;
            }
            count = list.size();

            return *this;
        };

        void push_back(T& obj) {
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