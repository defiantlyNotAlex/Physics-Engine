#include "headers/Colliders.hpp"
#include "headers/Camera.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

int main() {
    Node* root = new Node(nullptr, Transform({0, 0}));
    root->addChild(new RectCollider(root, Transform({-40, 40}), {40, 40}));
    root->addChild(new RectCollider(root, Transform({100, 100}, 0), {50, 50}));
    Camera* camera = (Camera*)root->addChild(new Camera(root, Transform({0, 0}, 0), 1));

    RectCollider* A = (RectCollider*)root->children[0];
    RectCollider* B = (RectCollider*)root->children[1];

    sf::Mouse mouse;

    //std::cout << A->overlappingBounds(B->transform.pos, B->getBounds()) << std::endl;
    std::cout << A->checkCol(B) << std::endl;

    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cout << "font failed to load" << std::endl;
    }

    sf::RenderWindow window(sf::VideoMode(800, 800), "physics");

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
        camera->transform.rot += 0.0001;
        if (B->checkPoint(camera->convertDisplaytoWorld(window, Vector2f(mpos.x, mpos.y)))) std::cout << "inside\n";    

        //B->transform.rot += 0.01f;
        //r_B->setRotation(B->transform.rot * (180.f/M_PI));

        window.clear();
        camera->drawRect(window, A->transform, A->getSize());
        camera->drawRect(window, B->transform, B->getSize());
        window.display();        
    }
    return 0;
}