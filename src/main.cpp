#include <SFML/Graphics.hpp>

#include "tank.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "wii tanks");
    window.setFramerateLimit(144);


    sf::RectangleShape square;
    square.setSize(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(sf::Vector2f(100, 100));

    Tank t(square);
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

        window.clear();
        // do things in here?

        window.draw(t.getBody());

        window.display();
    }
}
