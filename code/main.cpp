#include "headers/Collider.hpp"
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
    Camera* camera = (Camera*)root->addChild(new Camera(root, &window, Transform({0, 0}, 0), 1));
    MouseGrabber* mg = (MouseGrabber*)root->addChild(new MouseGrabber());

    Collider* A = (Collider*)root->addChild(new Collider(root, Transform({0, 140}, 0), new Rect({480, 40})));
    Collider* B = (Collider*)root->addChild(new Collider(root, Transform({20, 20}, 0), new Rect({50, 50})));

    PhysicsObject* P = (PhysicsObject*)root->addChild(new PhysicsObject(root, A->transform, A, 10, 333333));
    PhysicsObject* Q = (PhysicsObject*)root->addChild(new PhysicsObject(root, B->transform, B, 1, 3333));

    P->lockPosition = true;
    P->lockRotation = true;

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
        camera->drawShape(A->transform, A->getShape());
        camera->drawShape(B->transform, B->getShape());
        window.display(); 
        
              
    }
    return 0;
}