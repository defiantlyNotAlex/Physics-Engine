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
    world.mainCamera = (Camera*)world.root->addChild(new Camera(&window, Transform({0, 0}, 0), 1));
    MouseGrabber* mg = new MouseGrabber(&world);
    world.root->addChild(mg);

    PhysicsObject* Big = new PhysicsObject(Transform({0, 100}), new Collider(new Rect({1000, 100})), 1, 1, true);
    PhysicsObject* A = new PhysicsObject(Transform({10, -100}), new Collider(new Rect({20, 20})));
    PhysicsObject* B = new PhysicsObject(Transform({10, 40}), new Collider(new Circle(10)));
    world.root->addChild(world.newObject(A));
    world.root->addChild(world.newObject(B));
    world.root->addChild(world.newObject(Big));
    

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
        
        world.update(dt);
        
        world.physicsUpdate(dt, 5);

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

        Vector2f display = A->velocity;
        text.setString(std::to_string(display.x) + ", " + std::to_string(display.y));
        window.clear();
    
        window.draw(text);
        world.mainCamera->drawShape(A->transform, A->collider->getShape());
        world.mainCamera->drawShape(B->transform, B->collider->getShape());
        world.mainCamera->drawShape(Big->transform, Big->collider->getShape());
        window.display(); 
        
              
    }
    return 0;
}