#include "headers/Camera.hpp"
#include <iostream>
#define DEBUG

Camera::Camera(sf::RenderWindow * _window, Transform _transform, float _scale = 1) : Node(nullptr, _transform) {
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
void Camera::drawShape(Transform shapeTransform, Shape* shape, sf::Color colour, sf::Texture* sprite) {
    Circle* circ = dynamic_cast<Circle*>(shape);
    Rect* rect = dynamic_cast<Rect*>(shape);
    Polygon* poly = dynamic_cast<Polygon*>(shape);

    if (circ) {
        drawCirc(shapeTransform, circ->getRadius(), colour, sprite);
        return;
    }
    if (rect) {
        drawRect(shapeTransform, rect->getSize(), colour, sprite);
        return;
    }
    if (poly) {
        drawPolygon(shapeTransform, poly->getPoints(), colour, sprite);
        return;
    }
}
void Camera::drawRect(Transform rectTransform, Vector2f size, sf::Color colour, sf::Texture* sprite) {
    sf::RectangleShape rect(size * scale);
    rect.setOrigin(size * scale / 2.f);
    rect.setRotation(Maths::radToDegree(rectTransform.rot - transform.rot));
    rect.setFillColor(colour);
    if (sprite) {
        rect.setTexture(sprite);
    }
    rect.setPosition(convertWorldtoDisplay(rectTransform.pos));
    window->draw(rect);

    #ifdef DEBUG
    sf::Vertex line[] = {
        sf::Vertex(convertWorldtoDisplay(rectTransform.pos)), 
        sf::Vertex(convertWorldtoDisplay(rectTransform.pos + Maths::directionVector(rectTransform.rot) * (size.x / 2.f)))
    };
    line->color = sf::Color::Black;
    window->draw(line, 2, sf::Lines);
    #endif
}
void Camera::drawCirc(Transform circTransform, float radius, sf::Color colour, sf::Texture* sprite) {
    sf::CircleShape circ(radius * scale);

    circ.setOrigin(Maths::one() * radius * scale);
    circ.setRotation(Maths::radToDegree(circTransform.rot));
    circ.setFillColor(colour);
    if (sprite) {
        circ.setTexture(sprite);
    }
    circ.setPosition(convertWorldtoDisplay(circTransform.pos));
    window->draw(circ);

    #ifdef DEBUG
    sf::Vertex line[] = {
        sf::Vertex(convertWorldtoDisplay(circTransform.pos)), 
        sf::Vertex(convertWorldtoDisplay(circTransform.pos + Maths::directionVector(circTransform.rot) * radius))
    };
    line->color = sf::Color::Black;
    window->draw(line, 2, sf::Lines);
    #endif
}
void Camera::drawPolygon(Transform polygonTransform, vector<Vector2f> points, sf::Color colour, sf::Texture* sprite) {
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

    #ifdef DEBUG
    sf::Vertex line[] = {
        sf::Vertex(convertWorldtoDisplay(polygonTransform.pos)), 
        sf::Vertex(convertWorldtoDisplay(polygonTransform.convertLocaltoWorld(points[0])))
    };
    line->color = sf::Color::Black;
    window->draw(line, 2, sf::Lines);
    #endif
}
