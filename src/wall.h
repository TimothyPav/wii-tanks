#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <iostream>


using pixels = int;

// 0<=X<=31 AND 0<=y<=17
class Wall 
{
private:
    sf::RectangleShape wall;
    constexpr static pixels width = 60;
    constexpr static pixels height = 60;
    const sf::Color color;
    const int X;
    const int Y;
    bool m_hole = false;
    sf::Vector2f trueCoords = sf::Vector2f(X*width, Y*width);

    void setDefaults() {
        wall.setSize(sf::Vector2f(width, height));
        wall.setFillColor(color);
        wall.setPosition(trueCoords);
    }

public:
    Wall(int X, int Y, sf::Color color, bool hole=false) 
        : X(X)
        , Y(Y)
        , color(color)
        , m_hole(hole)
    {
        setDefaults();
        if (m_hole == true) wall.setFillColor(sf::Color::Green);
    };     

    sf::RectangleShape getWall() {
        return wall;
    }
    
    bool isHole() const 
    {
        return m_hole;
    }

};

#endif
