#include "headers/Colliders.hpp"
#include "headers/Camera.hpp"
#include "headers/PhysicsObject.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

int main() {
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cout << "font failed to load" << std::endl;
    }

    sf::Clock deltaClock;
    sf::RenderWindow window(sf::VideoMode(800, 800), "physics");   

    Node* root = new Node(nullptr, Transform({0, 0}));
    root->addChild(new RectCollider(root, Transform({40, 40}, 1), {40, 40}));
    root->addChild(new RectCollider(root, Transform({100, 100}, 1), {50, 50}));
    CircleCollider* D = (CircleCollider*)root->addChild(new CircleCollider(root, Transform(), 10));
    PolygonCollider* C = (PolygonCollider*)root->addChild(new PolygonCollider(root, Transform({-100, -100}), {{-10,-10},{0,-20},{10,-10},{10,10},{-10,10}}));
    Camera* camera = (Camera*)root->addChild(new Camera(root, &window, Transform({0, 0}, 0), 2));

    RectCollider* A = (RectCollider*)root->children[0];
    RectCollider* B = (RectCollider*)root->children[1];

    root->addChild(new PhysicsObject(root, root->transform, A));
    root->addChild(new PhysicsObject(root, root->transform, B));

    sf::Mouse mouse;

    //std::cout << A->overlappingBounds(B->transform.pos, B->getBounds()) << std::endl;
    std::cout << A->checkCol(B) << std::endl;
    
    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();
        float dt = deltaTime.asSeconds();

        root->update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        auto mpos = mouse.getPosition(window);
        auto mouseWorldPos = camera->convertDisplaytoWorld(Vector2f(mpos));
        if (mouse.isButtonPressed(sf::Mouse::Button::Left)) std::cout << mouseWorldPos.x << " " << mouseWorldPos.y << "\n";
        B->transform.rot += FloatUtils::half_pi * dt;

        C->transform.pos = mouseWorldPos;

        sf::Color aColour = sf::Color::White;
        sf::Color bColour = sf::Color::White;
        sf::Color cColour = sf::Color::White;
        sf::Color dColour = sf::Color::White;

        if (A->overlappingBounds(C)) aColour = sf::Color::Blue;    
        if (B->overlappingBounds(C)) bColour = sf::Color::Blue;    
        if (D->overlappingBounds(C)) dColour = sf::Color::Blue;
        
        if (A->checkCol(C)) aColour = sf::Color::Green;    
        if (B->checkCol(C)) bColour = sf::Color::Green;
        if (D->checkCol(C)) dColour = sf::Color::Green;


        window.clear();
        camera->drawRect(A->transform, A->getSize(), aColour);
        camera->drawRect(B->transform, B->getSize(), bColour);
        camera->drawPolygon(C->transform, C->getPoints(), cColour);
        camera->drawCirc(D->transform, D->getRadius(), dColour);
        window.display();        
    }
    return 0;
}