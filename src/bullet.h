#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "tank.h"

class Bullet
{
private:
    sf::RectangleShape body;
    int speed;
    int angle;


public:
    Bullet(float x, float y, int speed, int angle);
    
    void move(); 
};

#endif
