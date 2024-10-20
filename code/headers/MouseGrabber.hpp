#pragma once
#include "World.hpp"
#include <SFML/Window/Mouse.hpp>

class MouseGrabber : public Node {
    private:
        World* world;
        bool clickLastFrame;
    public:
        MouseGrabber(World* world);
        Vector2f mousePos;
        weak_ptr<PhysicsObject> grabbed;
        Vector2f posGrabbed;
        void updatePos(shared_ptr<Camera> camera);
        void update(float dt);
        void attemptGrab();
        void letGo();
};