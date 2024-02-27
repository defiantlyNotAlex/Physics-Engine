#pragma once
#include "Node.hpp"
#include <SFML/Graphics.hpp>

class Camera : public Node {
    private:
        /// @brief the scale of the camera display units per world unit
        float scale;
    public:
        Camera(Node* parent, Transform _transform, float _scale);
        Vector2f convertWorldtoDisplay(sf::RenderWindow &window, Vector2f world);
        Vector2f convertDisplaytoWorld(sf::RenderWindow &window, Vector2f display);

        void drawRect(sf::RenderWindow &window, Transform rectTransform, Vector2f size, sf::Color colour = sf::Color::White, sf::Texture* sprite = nullptr);

        void update(float dt);


};