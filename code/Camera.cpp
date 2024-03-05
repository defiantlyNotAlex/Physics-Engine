#include "headers/Camera.hpp"
#include <iostream>

Camera::Camera(Node* _parent, sf::RenderWindow * _window, Transform _transform, float _scale = 1) : Node(_parent, _transform) {
    scale = _scale;
    window = _window;
}

void Camera::update(float dt) {
    
}

const sf::RenderWindow* Camera::getWindow() {
    return window;
}

Vector2f Camera::convertWorldtoDisplay(Vector2f world) {
    const Vector2f local = transform.convertWorldtoLocal(world); 
    const Vector2f adjusted = local * scale;
    return adjusted + Vector2f(window->getSize().x, window->getSize().y)/2.f;
}
Vector2f Camera::convertDisplaytoWorld(Vector2f display) {
    const Vector2f adjusted = display - Vector2f(window->getSize().x, window->getSize().y)/2.f;
    const Vector2f local = adjusted / scale;
    return transform.convertLocaltoWorld(local);
}
void Camera::drawRect(Transform rectTransform, Vector2f size, sf::Color colour, sf::Texture* sprite) {
    sf::RectangleShape rect(size * scale);
    rect.setOrigin(size * scale / 2.f);
    rect.setRotation(FloatUtils::radToDegree(rectTransform.rot - transform.rot));
    rect.setFillColor(colour);
    if (sprite) {
        rect.setTexture(sprite);
    }
    rect.setPosition(convertWorldtoDisplay(rectTransform.pos));
    window->draw(rect);
}
void Camera::drawCirc(Transform circtTransform, float radius, sf::Color colour, sf::Texture* sprite) {
    sf::CircleShape circ(radius * scale);
    circ.setOrigin(VectorUtils::one() * radius * scale / 2.f);
    circ.setRotation(FloatUtils::radToDegree(circtTransform.rot));
    circ.setFillColor(colour);
    if (sprite) {
        circ.setTexture(sprite);
    }
    circ.setPosition(convertWorldtoDisplay(circtTransform.pos) - circ.getOrigin());
    window->draw(circ);
}
void Camera::drawPolygon(Transform polygonTransform, vector<Vector2f> & points, sf::Color colour, sf::Texture* sprite) {
    sf::ConvexShape polygon;
    polygon.setPointCount(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        polygon.setPoint(i, convertWorldtoDisplay(polygonTransform.convertLocaltoWorld(points[i])));
    }
    polygon.setFillColor(colour);
    if (sprite) {
        polygon.setTexture(sprite);
    }
    window->draw(polygon);
}
