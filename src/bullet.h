#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "wall.h"
#include "bomb.h"

class Tank;

enum class WallSide
{
    NODIRECTION,
    left,
    right,
    top,
    bottom,
};

class Bullet
{
private:
    sf::RectangleShape body;
    float speed;
    sf::Angle angle;
    int bounces = 3;
    Tank* owner;


public:
    Bullet(float x, float y, float speed, sf::Angle angle, Tank* owner);
    
    sf::RectangleShape getBody();
    bool collision(sf::RenderWindow& window, std::vector<Wall>& level, 
                   std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<std::unique_ptr<Tank>>& tanks);
    WallSide whichSide(Wall& wall);

    int getBounces() { return bounces; }
    void setZeroBounces() { bounces = 0; }

    void move(sf::RenderWindow& window, std::vector<Wall>& level, 
              std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<std::unique_ptr<Tank>>& tanks);
    Tank* decrementOwner(){ return owner; }
    
};


#endif
