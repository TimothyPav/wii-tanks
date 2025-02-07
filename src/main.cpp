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

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            t.moveTank(Direction::Up);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            t.moveTank(Direction::Down);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            t.moveTank(Direction::Left);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Right);
        }

        window.clear();
        // do things in here?

        for (int i{0}; i < currentLevel.size(); ++i)
            window.draw(currentLevel[i].getWall());
        
        t.rotateTurretBasedOnMouse(sf::Mouse::getPosition(window));
        window.draw(t.getTankBody());
        window.draw(t.getHeadBody());
        window.draw(t.getTurretBody());

        window.display();
    }
}
