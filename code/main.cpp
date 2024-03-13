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
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Physics Sandbox");   

    Node* root = new Node(nullptr, Transform({0, 0}));
    Camera* camera = (Camera*)root->addChild(new Camera(root, &window, Transform({0, 0}, 0), 1));
    MouseGrabber* mg = (MouseGrabber*)root->addChild(new MouseGrabber());

    Collider* A = (Collider*)root->addChild(new Collider(root, Transform({0, 300}, 0), new Rect({1000, 40})));
    Collider* B = (Collider*)root->addChild(new Collider(root, Transform({20, 20}, 0), new Rect({50, 50})));
    Collider* C = (Collider*)root->addChild(new Collider(root, Transform({40, 40}, 0), new Rect({50, 50})));
    //Collider* D = (Collider*)root->addChild(new Collider(root, Transform({20, -80}, 0), new Circle(50)));
    Collider* E = (Collider*)root->addChild(new Collider(root, Transform({-50, -100}, 0), new Polygon({{-20, -20}, {20, -20}, {25, 0}, {20, 20}, {-20, 20}, {-25, 0}})));
    //Collider* F = (Collider*)root->addChild(new Collider(root, Transform({80, -120}, 0), new Circle(20)));

    PhysicsObject* P = (PhysicsObject*)root->addChild(new PhysicsObject(root, A->transform, A, 10, 333333));
    PhysicsObject* Q = (PhysicsObject*)root->addChild(new PhysicsObject(root, B->transform, B, 1, 3333));
    PhysicsObject* R = (PhysicsObject*)root->addChild(new PhysicsObject(root, C->transform, C, 1, 3333));
    //PhysicsObject* S = (PhysicsObject*)root->addChild(new PhysicsObject(root, D->transform, D, 1, 3333));
    PhysicsObject* T = (PhysicsObject*)root->addChild(new PhysicsObject(root, E->transform, E, 1, 3333));
    //PhysicsObject* U = (PhysicsObject*)root->addChild(new PhysicsObject(root, F->transform, F, 1, 3333));

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            Transform t = Transform({i * 20, j * 20 - 400}, (float)rand()/(float)RAND_MAX);
            root->addChild(new PhysicsObject(root, t, new Collider(root, t, new Rect({10, 10})), 0.1, 333));
        }
    }
    P->lockPosition = true;
    //P->lockRotation = true;

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
        PhysicsObject::physicsUpdate(dt);

        
        if (time > 1) {
            
            frameCount = 0;
            time = 0;
        }

        text.setString("mousePos: " + std::to_string(mouseWorldPos.x) + ", " + std::to_string(mouseWorldPos.y));
        window.clear();
        for (PhysicsObject* obj : *PhysicsObject::objectList) {
            camera->drawShape(obj->transform, obj->collider->getShape());
        }
        window.draw(text);
        window.display(); 
        
              
    }
    return 0;
}