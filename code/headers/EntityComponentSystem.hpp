#pragma once
#include <vector>
#include "AABB.hpp"
#include "Transform.hpp"

template <class T>
class Component {
    private:
        std::vector<T> list;
    public:
        Component<T>();
        size_t add(Args... args);
        void remove(size_t id);

};


class EntityComponentSystem {
    private:
        std::vector<Transform> transforms;
        std::vector<AABB> boundingboxes;

    public:
        Transform* addComponent<Transform>(Vector2f position = {0, 0}, float rotation = 0f) {
            transforms.emplace_back(position, rotation);
            return &transform.back();
        }
};