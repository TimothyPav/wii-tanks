#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "utils.h"
#include "wall.h"
#include "bullet.h"
#include "bomb.h"
#include "Random.h"

/*
 * position
 * shoot function
 * move function/speed
 * body/square for now
 */

extern std::vector<std::unique_ptr<Tank>> tanks;

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
    std::vector<Bullet> bullets{};
    const int maxBullets{ 5 };

    bool isLevelTwoTank{ false };
    std::pair<Direction, Direction> dir{ getDirection(Random::get(0,7)) };

    bool isLevelThreeTank{ false };

    std::shared_ptr<Bomb> m_bomb{ nullptr };
    bool isBombPlaced{ false };

    bool isAlive{ true };

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
    Tank(const std::vector<Wall>& level, const float speed, const sf::Vector2f& position);

    std::vector<sf::RectangleShape> getBody();
    sf::RectangleShape getTankBody();
    sf::RectangleShape getTurretBody();
    sf::RectangleShape getHeadBody();
    std::vector<Bullet>& getBulletSet();

    int getMaxBullets() { return maxBullets; }
    std::shared_ptr<Bomb> getBomb() { return m_bomb; }

    float getX();
    float getY();
    void updateMoveValues(float xSpeed, float ySpeed);
    void moveTank(Direction dir, Direction dir2 = Direction::NODIRECTION);
    bool checkBoundaries(Direction dir, Direction dir2 = Direction::NODIRECTION);
    bool checkRotation(Direction dir, Direction dir2 = Direction::NODIRECTION); 
    void rotateTurretBasedOnMouse(sf::Vector2i mousePosition);
    void shoot();
    void plantBomb(); 
    void resetBomb() { isBombPlaced = false; }
    
    void kill() { isAlive = false; }
    bool getIsAlive() { return isAlive; }

    void setLevelTwoTank(){ isLevelTwoTank = true; }
    bool getIsLevelTwoTank(){ return isLevelTwoTank; }
    std::pair<Direction, Direction> getDir(){ return dir; }
    void changeDir(){ dir = getDirection(Random::get(0,7)); }

    void setLevelThreeTank(){ isLevelThreeTank = true; }
    bool getIsLevelThreeTank(){ return isLevelThreeTank; }
    // ai tank motions/member functions
    void rotateTurretAtPlayer(const Tank& player);

    void moveTowardsPlayer(const Tank& player);


    void getTankCoords() const;
    void test() const;

    // afterthought variables
    bool state1{ false };
    int everySecond{};
};

#endif
