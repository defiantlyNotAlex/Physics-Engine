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
    world.mainCamera = (Camera*)world.root->addChild(new Camera(&window, Transform({0, 0}, 0), 2));
    MouseGrabber* mg = new MouseGrabber(&world);
    world.root->addChild(mg);

    PhysicsObject* Big = new PhysicsObject(Transform({0, 100}), new Collider(new Rect({2000, 100})), 1, 1, true);
    PhysicsObject* A = new PhysicsObject(Transform({-20, -100}), new Collider(new Circle(10)), 1, 16);
    PhysicsObject* B = new PhysicsObject(Transform({-20, -40}), new Collider(new Circle(10)), 1, 16);
    PhysicsObject* C = new PhysicsObject(Transform({-40, -40}), new Collider(new Rect({20, 20})), 1, 16);
    world.root->addChild(world.newObject(A));
    world.root->addChild(world.newObject(B));
    world.root->addChild(world.newObject(C));
    world.root->addChild(world.newObject(Big));

    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            world.root->addChild(world.newObject(new PhysicsObject(Transform({(float)i*20, -(float)j*20}), new Collider(new Circle(10)), 1, 16)));
        }
    }
    

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
        
        world.physicsUpdate(dt, 4);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        

        
        if (time > 1) {
            text.setString("FPS: " + std::to_string(frameCount));
            frameCount = 0;
            time = 0;
        }
        
        //Vector2f display = B->velocity;
        //text.setString(std::to_string(display.x) + ", " + std::to_string(display.y) + "Rotation: " + std::to_string(B->angularVelocity));
        window.clear();
    
        window.draw(text);
        for (PhysicsObject* obj : world.objectList) {
            world.mainCamera->drawShape(obj->transform, obj->collider->getShape());
        }
        world.mainCamera->drawShape(A->transform, A->collider->getShape());
        world.mainCamera->drawShape(B->transform, B->collider->getShape());
        world.mainCamera->drawShape(Big->transform, Big->collider->getShape());
        window.display(); 
        
              
    }
    return 0;
}