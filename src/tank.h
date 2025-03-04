#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <array>
#include <vector>

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
extern std::vector<Bullet> bullets;
extern std::vector<std::shared_ptr<Bomb>> bombs;

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
    int maxBullets{ 5 };

    bool isLevelTwoTank{ false }; // moves randomly every 1 second
    bool isLevelThreeTank{ false }; // moves towards player 1 direction per second
    bool isLevelFourTank{ false }; // 1 bullet and can place 3 bombs moves randomly and has speed 2
    bool isLevelFiveTank{ false }; // follow player, quick, 1 bomb, 5 bullets


    std::pair<Direction, Direction> dir{ getDirection(Random::get(0,7)) };

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

    ~Tank();

    std::vector<sf::RectangleShape> getBody();
    sf::RectangleShape getTankBody();
    sf::RectangleShape getTurretBody();
    sf::RectangleShape getHeadBody();

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
    void resetBomb() { 
        isBombPlaced = false;
        m_bomb = nullptr;
    }

    int currentBullets{ 0 };

    bool getIsBombPlaced() const { return isBombPlaced; }
    
    void kill() { isAlive = false; }
    bool getIsAlive() { return isAlive; }

    std::pair<Direction, Direction> getDir(){ return dir; }
    void changeDir(){ dir = getDirection(Random::get(0,7)); }

    void setLevelTwoTank(){ 
        isLevelTwoTank = true; 
        maxBullets = 3;
    }
    bool getIsLevelTwoTank(){ return isLevelTwoTank; }

    void setLevelThreeTank(){ 
        isLevelThreeTank = true; 
        maxBullets = 3;
    }
    bool getIsLevelThreeTank(){ return isLevelThreeTank; }

    void setLevelFourTank() {
        isLevelFourTank = true;
        maxBullets = 1;
    }
    bool getIsLevelFourTank(){ return isLevelFourTank; }
    std::vector<std::shared_ptr<Bomb>> m_bombVector{};

    void setLevelFiveTank() {
        isLevelFiveTank = true;
        maxBullets = 5;
    }
    bool getIsLevelFiveTank(){ return isLevelFiveTank; }

    // ai tank motions/member functions
    void rotateTurretAtPlayer(const Tank& player);

    void moveTowardsPlayer(const Tank& player);

    void plantBombEnemy(int maxBombs); 

    void getTankCoords() const;
    void test() const;

    // afterthought variables
    bool state1{ false };
    int everySecond{};
};

#endif
