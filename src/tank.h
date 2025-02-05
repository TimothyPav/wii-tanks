#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>

#include "utils.h"
#include "wall.h"
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
    sf::RectangleShape turret;
    std::vector<sf::RectangleShape> tankShapes;
    float speed;
    float x;
    float y;
    std::vector<Wall> level;

    void setDefaults() {
        body.setSize(sf::Vector2f(50, 50));
        body.setFillColor(sf::Color::Red);
        body.setPosition(sf::Vector2f(100, 100));

        turret.setSize(sf::Vector2f(50, 5));
        turret.setFillColor(sf::Color::Cyan);
        turret.setPosition(sf::Vector2f(100, 100));
        
        tankShapes.push_back(body);
        tankShapes.push_back(turret);
    }

public:
    Tank(const sf::RectangleShape& body, const float& speed, const std::vector<Wall>& level);

    std::vector<sf::RectangleShape>& getBody();

    float getX();
    float getY();
    void moveTank(Direction dir);
    bool checkBoundaries(Direction dir);
    
    void test() const;
};

#endif
