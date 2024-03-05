#pragma once
#include "PhysicsObject.hpp"
#include "Camera.hpp"
#include <SFML/Window/Mouse.hpp>

class MouseGrabber : public Node {
    private:
        bool clickLastFrame;
    public:
        Vector2f mousePos;
        PhysicsObject* grabbed;
        Vector2f posGrabbed;
        void updatePos(Camera * camera);
        void update(float dt);
        void attemptGrab();
        void letGo();
};