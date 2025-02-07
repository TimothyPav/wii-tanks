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
    sf::RectangleShape head;
    std::vector<sf::RectangleShape> tankShapes;
    float speed;
    float x;
    float y;
    std::vector<Wall> level;

    void setDefaults() {
        body.setSize(sf::Vector2f(50, 50));
        body.setFillColor(sf::Color::Red);
        body.setPosition(sf::Vector2f(100, 100));

        head.setSize(sf::Vector2f(30, 30));
        head.setFillColor(sf::Color::Cyan);
        sf::Vector2f headSize = body.getSize();
        head.setPosition(sf::Vector2f(100 + headSize.x / 2.0f, 100 + headSize.y / 2.0f));

        turret.setSize(sf::Vector2f(50, 10));
        turret.setPosition(sf::Vector2f({body.getPosition().x+body.getSize().x/2.0f, body.getPosition().y+body.getSize().y/2.0f}));
        sf::Vector2f size = turret.getSize();
        turret.setFillColor(sf::Color::Cyan);

        tankShapes.push_back(body);
        tankShapes.push_back(turret);
    }

public:
    Tank(const sf::RectangleShape& body, const float& speed, const std::vector<Wall>& level);

    std::vector<sf::RectangleShape> getBody();
    sf::RectangleShape getTankBody();
    sf::RectangleShape getTurretBody();
    sf::RectangleShape getHeadBody();

    float getX();
    float getY();
    void moveTank(Direction dir);
    bool checkBoundaries(Direction dir);
    void rotateTurretBasedOnMouse(sf::Vector2i mousePosition);
    
    void getTankCoords() const;
    void test() const;
};

#endif
