#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <iostream>

using pixels = int;

template <pixels X, pixels Y> // 0<=X<=31 AND 0<=y<=17
class Wall 
{
private:
    sf::RectangleShape wall;
    constexpr static pixels width = 60;
    constexpr static pixels height = 60;
    const sf::Color color;
    constexpr static sf::Vector2f trueCoords = sf::Vector2f(X*width, Y*width);

    void setDefaults() {
        wall.setSize(sf::Vector2f(width, height));
        wall.setFillColor(color);
        wall.setPosition(trueCoords);
    }

public:
    Wall(sf::Color color) : color(color) {
        setDefaults();
    };     

    sf::RectangleShape getWall() {
        return wall;
    }
};

#endif
