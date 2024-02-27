#pragma once
#include "Node.hpp"

class Camera : public Node {
    private:
        /// @brief the scale of the camera display units per world unit
        float scale;
    public:
        Camera(Node* parent, Transform _transform, float scale);
        Vector2i convertWorldtoDisplay(Vector2f world);
        Vector2f convertDisplaytoWorld(Vector2f world);

        void update(float dt);


};