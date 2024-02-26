#include "headers/RectCollider.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    RectCollider* A = new RectCollider(Transform({0, 0}), {40, 40});
    RectCollider* B = new RectCollider(Transform({40, 30}, 1.f), {40, 40});

    //std::cout << A->overlappingBounds(B->transform.pos, B->getBounds()) << std::endl;
    std::cout << A->checkCol(B) << std::endl;

    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cout << "font failed to load" << std::endl;
    }
    
    sf::RenderWindow window(sf::VideoMode(200, 200), "physics");
    sf::RectangleShape* r_A = new sf::RectangleShape(A->getSize());
    sf::RectangleShape* r_B = new sf::RectangleShape(B->getSize());

    r_A->setPosition(A->transform.pos);
    r_B->setPosition(B->transform.pos);

    r_A->setRotation(A->transform.rot * (180.f/M_PI));
    r_B->setRotation(B->transform.rot * (180.f/M_PI));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(*r_A);
        window.draw(*r_B);
        window.display();        
    }
    return 0;
}