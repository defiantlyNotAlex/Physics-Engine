#include "headers/Colliders.hpp"
#include "headers/Camera.hpp"
#include "headers/PhysicsObject.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>

int main() {
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cout << "font failed to load" << std::endl;
    }

    sf::Clock deltaClock;
    sf::RenderWindow window(sf::VideoMode(800, 800), "physics");   

    Node* root = new Node(nullptr, Transform({0, 0}));
    root->addChild(new RectCollider(root, Transform({0, 0}, 0), {40, 40}));
    root->addChild(new RectCollider(root, Transform({100, 100}, 1), {50, 50}));
    CircleCollider* D = (CircleCollider*)root->addChild(new CircleCollider(root, Transform({-200, -100}), 10));
    PolygonCollider* C = (PolygonCollider*)root->addChild(new PolygonCollider(root, Transform({-100, -100}), {{-10,-10},{0,-20},{10,-10},{10,10},{-10,10}}));
    Camera* camera = (Camera*)root->addChild(new Camera(root, &window, Transform({0, 0}, 0), 2));

    for (int i = 0; i < 5; i++) {
        for (int j = 0 ; j < 5; j++) {
            PhysicsObject* A = (PhysicsObject*)root->addChild(new PhysicsObject(root, Transform({i*-10, j*-10}), new RectCollider(A, Transform({i*-10, j*-10}), {10, 10})));
        }
    }

    RectCollider* A = (RectCollider*)root->children[0];
    RectCollider* B = (RectCollider*)root->children[1];

    PhysicsObject* P = (PhysicsObject*)root->addChild(new PhysicsObject(root, A->transform, A));
    PhysicsObject* Q = (PhysicsObject*)root->addChild(new PhysicsObject(root, B->transform, B));
    PhysicsObject* M = (PhysicsObject*)root->addChild(new PhysicsObject(root, C->transform, C));

    sf::Mouse mouse;

    sf::Text text;
    text.setFont(font);
    float time = 0;
    int64_t frameCount = 0;
    bool pressedLastFrame = false;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();
        float dt = deltaTime.asSeconds();
        time += dt;
        frameCount++;

        root->update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        auto mpos = mouse.getPosition(window);
        auto mouseWorldPos = camera->convertDisplaytoWorld(Vector2f(mpos));

        if (mouse.isButtonPressed(sf::Mouse::Button::Left) && !pressedLastFrame) {
            pressedLastFrame = true;
            M->setPosition(mouseWorldPos);
        } else if (!mouse.isButtonPressed(sf::Mouse::Button::Left)) {
            pressedLastFrame = false;
        }

        

        sf::Color aColour = sf::Color::White;
        sf::Color bColour = sf::Color::White;
        sf::Color cColour = sf::Color::White;
        sf::Color dColour = sf::Color::White;

        if (A->overlappingBounds(C)) aColour = sf::Color::Blue;    
        if (B->overlappingBounds(C)) bColour = sf::Color::Blue;    
        if (D->overlappingBounds(C)) dColour = sf::Color::Blue;
        
        if (P->getOverlap()) aColour = sf::Color::Green;    
        if (Q->getOverlap()) bColour = sf::Color::Green;
        if (D->checkCol(C)) dColour = sf::Color::Green;
        //M->applyForce(dt, M->collider->getOverlap(A).normal, M->transform.pos);
        M->setPosition(M->getPosition() + M->collider->getOverlap(A).normal * M->collider->getOverlap(A).depth);

        
        if (time > 1) {
            text.setString(std::to_string(frameCount));
            frameCount = 0;
            time = 0;
            std::cout << C->getOverlap(A).normal.x << "," << C->getOverlap(A).normal.y << "," << C->getOverlap(A).depth << std::endl;
        }
        
        window.clear();
        
        window.draw(text);
        camera->drawRect(A->transform, A->getSize(), aColour);
        camera->drawRect(B->transform, B->getSize(), bColour);
        camera->drawPolygon(C->transform, C->getPoints(), cColour);
        camera->drawCirc(D->transform, D->getRadius(), dColour);
        window.display();        
    }
    return 0;
}