#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>

#include "utils.h"
/*
 * position
 * shoot function
 * move function/speed
 * body/square for now
 */

class Tank
{
private:
    sf::RectangleShape body;
    float speed;
    float x;
    float y;

    void setDefaults() {
        body.setSize(sf::Vector2f(50, 50));
        body.setFillColor(sf::Color::Red);
        body.setPosition(sf::Vector2f(100, 100));
    }

public:
    Tank(const sf::RectangleShape& body, const float& speed);

    sf::RectangleShape& getBody();

    float getX();
    float getY();
    void moveTank(Direction dir);
    bool checkBoundaries(Direction dir);
    
    void test() const;
};

#endif
