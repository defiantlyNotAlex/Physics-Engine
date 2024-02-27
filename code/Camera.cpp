#include "headers/Camera.hpp"
#include <iostream>

Camera::Camera(Node* _parent, Transform _transform, float _scale = 1) : Node(_parent, _transform) {
    scale = _scale;
}

void Camera::update(float dt) {

}

Vector2f Camera::convertWorldtoDisplay(sf::RenderWindow &window, Vector2f world) {
    const Vector2f local = transform.convertWorldtoLocal(world);
    const Vector2f adjusted = VectorUtils::componentProd(local * scale, Vector2f(1, -1));
    return adjusted + Vector2f(window.getSize().x, window.getSize().y)/2.f;
}
Vector2f Camera::convertDisplaytoWorld(sf::RenderWindow &window, Vector2f display) {
    const Vector2f adjusted = display - Vector2f(window.getSize().x, window.getSize().y)/2.f;
    const Vector2f local = VectorUtils::componentProd(adjusted / scale, Vector2f(1, -1));
    return transform.convertLocaltoWorld(local);
}
void Camera::drawRect(sf::RenderWindow &window, Transform rectTransform, Vector2f size, sf::Color colour, sf::Texture* sprite) {
    sf::RectangleShape rect(size * scale);
    rect.setOrigin(size * scale / 2.f);
    rect.setRotation(FloatUtils::radToDegree(rectTransform.rot + transform.rot));
    rect.setFillColor(colour);
    if (sprite) {
        rect.setTexture(sprite);
    }
    rect.setPosition(convertWorldtoDisplay(window, rectTransform.pos));
    window.draw(rect);
}