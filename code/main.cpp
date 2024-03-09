#include "headers/Colliders.hpp"
#include "headers/Camera.hpp"
#include "headers/PhysicsObject.hpp"
#include "headers/MouseGrab.hpp"
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
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "physics");   

    Node* root = new Node(nullptr, Transform({0, 0}));
    root->addChild(new RectCollider(root, Transform({0, 140}, 0), {480, 40}));
    root->addChild(new RectCollider(root, Transform({20, 20}, 0), {50, 50}));
    RectCollider* E = (RectCollider*)root->addChild(new RectCollider(root, Transform({-40, -10}, 0), {100, 100}));
    CircleCollider* D = (CircleCollider*)root->addChild(new CircleCollider(root, Transform({-200, -100}), 10));
    PolygonCollider* C = (PolygonCollider*)root->addChild(new PolygonCollider(root, Transform({-100, -100}), {{-20,-20},{0,-40},{20,-20},{20,20},{-20,20}}));
    Camera* camera = (Camera*)root->addChild(new Camera(root, &window, Transform({0, 0}, 0), 1));
    MouseGrabber* mg = (MouseGrabber*)root->addChild(new MouseGrabber());
    for (int i = 0; i < 5; i++) {
        for (int j = 0 ; j < 5; j++) {
            //PhysicsObject* A = (PhysicsObject*)root->addChild(new PhysicsObject(root, Transform({i*-10, j*-10}), new RectCollider(A, Transform({i*-10, j*-10}), {10, 10})));
        }
    }

    RectCollider* A = (RectCollider*)root->children[0];
    RectCollider* B = (RectCollider*)root->children[1];

    PhysicsObject* P = (PhysicsObject*)root->addChild(new PhysicsObject(root, A->transform, A, 10, 333333));
    PhysicsObject* Q = (PhysicsObject*)root->addChild(new PhysicsObject(root, B->transform, B, 1, 3333));
    PhysicsObject* M = (PhysicsObject*)root->addChild(new PhysicsObject(root, D->transform, D, 1, 1000));
    PhysicsObject* K = (PhysicsObject*)root->addChild(new PhysicsObject(root, C->transform, C, 1, 1000));
    P->lockPosition = true;
    P->lockRotation = true;
    //P->material.elasticity = 2;
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


        auto mpos = mouse.getPosition(window);
        auto mouseWorldPos = camera->convertDisplaytoWorld(Vector2f(mpos));        
        
        time += dt;
        frameCount++;


        
        mg->updatePos(camera);
        root->update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        

        
        if (time > 1) {
            
            frameCount = 0;
            time = 0;
        }

        text.setString("mousePos: " + std::to_string(mouseWorldPos.x) + ", " + std::to_string(mouseWorldPos.y));
        window.clear();
    
        window.draw(text);
        camera->drawRect(A->transform, A->getSize());
        //camera->drawRect(E->transform, E->getSize());
        camera->drawRect(B->transform, B->getSize());
        camera->drawPolygon(C->transform, C->getPoints());
        camera->drawCirc(D->transform, D->getRadius());
        window.display(); 
        
              
    }
    return 0;
}