#include "headers/MouseGrab.hpp"

MouseGrabber::MouseGrabber(World* _world) : Node(nullptr, Transform()) {
    world = _world;
}

void MouseGrabber::updatePos(Camera * camera) {
    auto mpos = sf::Mouse::getPosition(*camera->getWindow());
    mousePos = world->mainCamera->convertDisplaytoWorld(Vector2f(mpos));
}
void MouseGrabber::update(float dt) {
    if (grabbed) {
        Vector2f grab = grabbed->transform.convertLocaltoWorld(posGrabbed);
        grabbed->applyForce(dt, 2.f * (mousePos - grab), grab);
        
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!clickLastFrame) {
            attemptGrab();
        }
        clickLastFrame = true;
    } else {
        clickLastFrame = false;
        letGo();
    }

}
void MouseGrabber::attemptGrab() {
    grabbed = world->getObjectAtPoint(mousePos);
    if (grabbed) {
        posGrabbed = grabbed->transform.convertWorldtoLocal(mousePos);
    }
}
void MouseGrabber::letGo() {
    grabbed = nullptr;
}