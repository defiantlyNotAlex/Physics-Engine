#pragma once
#include "Node.hpp"
#include "Shape.hpp"
#include <SFML/Graphics.hpp>

class Camera : public Node {
    private:
        /// @brief the scale of the camera display units per world unit
        float scale;
        sf::RenderWindow* window;
    public:

        const sf::RenderWindow* getWindow();
    
        Camera(sf::RenderWindow * window, Transform _transform, float _scale);
        Vector2f convertWorldtoDisplay(Vector2f world);
        Vector2f convertDisplaytoWorld(Vector2f display);

        void drawShape(Transform shapeTransform, Shape* shape, sf::Color colour = sf::Color::White, sf::Texture* sprite = nullptr);
        void drawRect(Transform rectTransform, Vector2f size, sf::Color colour = sf::Color::White, sf::Texture* sprite = nullptr);
        void drawCirc(Transform circTransform, float radius,  sf::Color colour = sf::Color::White, sf::Texture* sprite = nullptr);
        void drawPolygon(Transform polygonTransform, vector<Vector2f> points,  sf::Color colour = sf::Color::White, sf::Texture* sprite = nullptr);

        void update(float dt);
};