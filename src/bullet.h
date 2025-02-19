#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "wall.h"
#include "bomb.h"

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
    int speed;
    sf::Angle angle;
    int bounces = 3;


public:
    Bullet(float x, float y, int speed, sf::Angle angle);
    
    sf::RectangleShape getBody();
    bool collision(sf::RenderWindow& window, std::vector<Wall>& level, std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<Tank>& tanks);
    WallSide whichSide(Wall& wall);

    int getBounces() { return bounces; }

    void move(sf::RenderWindow& window, std::vector<Wall>& level, std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<Tank>& tanks);
};

#endif
