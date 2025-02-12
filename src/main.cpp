#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Window/Mouse.hpp>

#include "tank.h"
#include "utils.h"
#include "wall.h"


// TODO: Obstacles/walls object to put on the map!
// All the blocks will be 24x24

std::vector<Wall> level_1();

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920, 1080}), "wii tanks");
    window.setFramerateLimit(144);

    sf::RectangleShape square;
    square.setSize(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(sf::Vector2f(100, 100));

    sf::Color color = sf::Color::Green;

    // Tank t(square, 5);
    std::vector<Wall> currentLevel = level_1();
    Tank t (square, 5, currentLevel);
    bool isMousePressed { false };

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                isMousePressed = false;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            // std::cout << "Both UP and LEFT are pressed";
            t.moveTank(Direction::Up, Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Up, Direction::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Down, Direction::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            t.moveTank(Direction::Down, Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            // std::cout << "Only UP is pressed";
            t.moveTank(Direction::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            t.moveTank(Direction::Down);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            t.moveTank(Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Right);
        }

        // MOUSE CLICK DETECTED HERE
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !isMousePressed)
        {
            // std::cout << "Bullets in set: " << t.getBulletSet().size() << '\n';
            isMousePressed = true;
            t.shoot();
        }

        window.clear();
        // do things in here?

        for (int i{0}; i < currentLevel.size(); ++i)
            window.draw(currentLevel[i].getWall());
        
        t.rotateTurretBasedOnMouse(sf::Mouse::getPosition(window));
        window.draw(t.getTankBody());
        window.draw(t.getHeadBody());
        window.draw(t.getTurretBody());

        // draw bullets
        for (Bullet& bullet : t.getBulletSet()) {
            bullet.move();
            window.draw(bullet.getBody());
        }

        window.display();
    }
}
