#include "headers/Colliders.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

int main() {
    RectCollider* A = new RectCollider(Transform({40, 40}), {40, 40});
    RectCollider* B = new RectCollider(Transform({140, 30}, 2.f), {40, 40});
    CircleCollider* C = new CircleCollider(Transform({120, 120}), 30);

    sf::Mouse mouse;

    //std::cout << A->overlappingBounds(B->transform.pos, B->getBounds()) << std::endl;
    std::cout << A->checkCol(B) << std::endl;
    std::cout << A->checkCol(C) << std::endl;

    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cout << "font failed to load" << std::endl;
    }

    
    sf::RenderWindow window(sf::VideoMode(600, 600), "physics");
    sf::RectangleShape* r_A = new sf::RectangleShape(A->getSize());
    sf::RectangleShape* r_B = new sf::RectangleShape(B->getSize());
    sf::CircleShape c_C (C->getRadius());

    r_A->setPosition(A->transform.pos - A->getSize() / 2.f);
    c_C.setPosition(C->transform.pos - C->getRadius() * VectorUtils::one());

    r_A->setRotation(A->transform.rot * (180.f/M_PI));
    r_B->setRotation(B->transform.rot * (180.f/M_PI));
    r_B->setOrigin(r_B->getSize()/2.f);
    r_B->setPosition(B->transform.pos);
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        auto mpos = mouse.getPosition(window);
        //std::cout << mpos.x << " " << mpos.y << "\n";
        std::cout << B->checkPoint(Vector2f(mpos.x, mpos.y)) << "\n";    

        //B->transform.rot += 0.01f;
        //r_B->setRotation(B->transform.rot * (180.f/M_PI));

        window.clear();
        window.draw(*r_A);
        window.draw(*r_B);
        window.draw(c_C);
        window.display();        
    }
    return 0;
}