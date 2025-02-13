#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>


class Bullet
{
private:
    sf::RectangleShape body;
    int speed;
    sf::Angle angle;


public:
    Bullet(float x, float y, int speed, sf::Angle angle);
    
    sf::RectangleShape getBody();

    void move(); 
};

#endif
