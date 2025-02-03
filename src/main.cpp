#include <SFML/Graphics.hpp>
#include <iostream>

#include "tank.h"
#include "utils.h"
#include "wall.h"

// TODO: Obstacles/walls object to put on the map!
// All the blocks will be 24x24

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920, 1080}), "wii tanks");
    window.setFramerateLimit(144);

    sf::RectangleShape square;
    square.setSize(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(sf::Vector2f(100, 100));

    sf::Color color = sf::Color::Green;
    Wall<0, 0> w{ color }; 

    Tank t(square, 5);
    t.test();

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

        window.draw(w.getWall());
        window.draw(t.getBody());

        window.display();
    }
}
