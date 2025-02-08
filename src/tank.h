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
        body.setOrigin(sf::Vector2f(25, 25));
        body.setSize(sf::Vector2f(50, 50));
        body.setFillColor(sf::Color::Red);
        body.setPosition(sf::Vector2f(100, 100));

        head.setSize(sf::Vector2f(30, 30));
        head.setPosition(sf::Vector2f(body.getPosition().x + 25, body.getPosition().y + 25)); // magic number 25 just works for centering the head
        head.setFillColor(sf::Color::Cyan);

        turret.setSize(sf::Vector2f(50, 10));
        turret.setPosition(sf::Vector2f({body.getPosition().x+body.getSize().x/2.0f, body.getPosition().y+body.getSize().y/2.0f}));
        turret.setFillColor(sf::Color::Cyan);

        body.move(sf::Vector2f(25, 25));

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
    void moveTank(Direction dir, Direction dir2 = Direction::NODIRECTION);
    bool checkBoundaries(Direction dir, Direction dir2 = Direction::NODIRECTION);
    bool checkRotation(Direction dir, Direction dir2 = Direction::NODIRECTION); 
    void rotateTurretBasedOnMouse(sf::Vector2i mousePosition);
    
    void getTankCoords() const;
    void test() const;
};

#endif
