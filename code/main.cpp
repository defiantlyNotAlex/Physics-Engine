#include "headers/Collider.hpp"
#include "headers/Camera.hpp"
#include "headers/PhysicsObject.hpp"
#include "headers/MouseGrab.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <string>
#include "headers/World.hpp"

int main() {
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cout << "font failed to load" << std::endl;
    }

    sf::Clock deltaClock;
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "physics");   

    World world;
    world.root = new Node(nullptr, Transform({0, 0}));
    world.mainCamera = (Camera*)world.root->addChild(new Camera(world.root, &window, Transform({0, 0}, 0), 1));
    MouseGrabber* mg = new MouseGrabber(&world);
    world.root->addChild(mg);

    PhysicsObject* A = new PhysicsObject(world.root, Transform({10, 10}), new Collider(world.root, Transform({10, 10}), new Circle(10)));
    PhysicsObject* B = new PhysicsObject(world.root, Transform({10, 10}), new Collider(world.root, Transform({10, 10}), new Circle(10)));
    world.root->addChild(A);
    world.root->addChild(B);
    world.newObject(A);
    world.newObject(B);


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
        auto mouseWorldPos = world.mainCamera->convertDisplaytoWorld(Vector2f(mpos));        
        
        time += dt;
        frameCount++;


        
        mg->updatePos(world.mainCamera);
        world.root->update(dt);

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
        world.mainCamera->drawShape(A->transform, A->collider->getShape());
        world.mainCamera->drawShape(B->transform, B->collider->getShape());
        window.display(); 
        
              
    }
    return 0;
}