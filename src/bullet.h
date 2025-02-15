#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "wall.h"


class Bullet
{
private:
    sf::RectangleShape body;
    int speed;
    sf::Angle angle;


public:
    Bullet(float x, float y, int speed, sf::Angle angle);
    
    sf::RectangleShape getBody();
    bool collision(sf::RenderWindow& window, std::vector<Wall>& level) const;

    void move(sf::RenderWindow& window, std::vector<Wall>& level); 
};

#endif
