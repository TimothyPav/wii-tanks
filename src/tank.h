#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <array>
#include <vector>

#include "utils.h"
#include "wall.h"
#include "bullet.h"
#include "bomb.h"
#include "Random.h"
#include "Animation.h"

/*
 * position
 * shoot function
 * move function/speed
 * body/square for now
 */

extern std::vector<std::unique_ptr<Tank>> tanks;
extern std::vector<std::unique_ptr<Bullet>> bullets;
extern std::vector<std::shared_ptr<Bomb>> bombs;
extern std::vector<Wall> currLevel;
extern std::vector<sf::RectangleShape> treads;

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
    int odds{ 300 };

    int distance{};

    bool isLevelTwoTank{ false }; // moves randomly every 1 second
    bool isLevelThreeTank{ false }; // moves towards player 1 direction per second
    bool isLevelFourTank{ false }; // 1 bullet and can place 3 bombs moves randomly and has speed 2
    bool isLevelFiveTank{ false }; // follow player, quick, 1 bomb, 5 bullets


    std::pair<Direction, Direction> dir{ getDirection(Random::get(0,7)) };

    std::shared_ptr<Bomb> m_bomb{ nullptr };
    bool isBombPlaced{ false };

    bool isAlive{ true };
    bool isPlayer{ false };

    sf::Texture explosionTexture;
    Animation animation = Animation(&explosionTexture, sf::Vector2u(6, 1), .1);
    bool animationFinished{ false };

    void setDefaults() {
        body.setOrigin(sf::Vector2f(25, 25));
        body.setSize(sf::Vector2f(50, 50));
        body.setPosition(sf::Vector2f(100, 100));

        head.setSize(sf::Vector2f(30, 30));
        head.setPosition(sf::Vector2f(body.getPosition().x + 25, body.getPosition().y + 25)); // magic number 25 just works for centering the head

        turret.setSize(sf::Vector2f(50, 10));
        turret.setPosition(sf::Vector2f({body.getPosition().x+body.getSize().x/2.0f, body.getPosition().y+body.getSize().y/2.0f}));


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

    int getDistance(){ return distance; }

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

    void setPos(sf::Vector2f pos)
    {
       body.setPosition(pos); 
       turret.setPosition(pos); 
       head.setPosition(pos); 
    }

    void resetRotation() {
        body.setRotation(sf::degrees(0));
        head.setRotation(sf::degrees(180));
        turret.setRotation(sf::degrees(180)); 
    }

    int currentBullets{ 0 };

    bool getIsBombPlaced() const { return isBombPlaced; }
    
    void kill() { 
        isAlive = false;
    
        // Save the original center position in world coordinates
        sf::Vector2f originalCenter = body.getPosition();
    
        // Set the origin to the center of the shape before resizing
        sf::Vector2f originalSize = body.getSize();
        body.setOrigin(originalSize / 2.0f);
    
        // Now set the new size
        body.setSize({96, 96});
    
        // Update the origin to the center of the new size
        body.setOrigin({96.0f / 2.0f, 96.0f / 2.0f});
    
        // Position remains the same because we're using the center as the origin
        body.setPosition(originalCenter);
    
        body.setTexture(&explosionTexture);
        animation = Animation(&explosionTexture, sf::Vector2u(6, 1), .05);
        // animate(0);
    }

    void revive() { isAlive = true; }
    bool getIsAlive() { return isAlive; }
    void setPlayer() { isPlayer = true; }
    bool getIsPlayer() { return isPlayer; }

    void giveBodyOutline()
    {
        // body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Red);
        body.setOutlineThickness(3);
    }

    void resetPlayerBodySize()
    {
        if (!isPlayer) return;

        body.setSize({50, 50});
        body.setOrigin({25, 25});
    }

    void animate(float deltaTime);
    bool isAnimationFinished(){ return animationFinished; }

    std::pair<Direction, Direction> getDir(){ return dir; }
    void changeDir(){ dir = getDirection(Random::get(0,7)); }

    void setLevelTwoTank(){ 
        isLevelTwoTank = true; 
        maxBullets = 3;
        speed = 1.5;
    }
    bool getIsLevelTwoTank(){ return isLevelTwoTank; }

    void setLevelThreeTank(){ 
        isLevelThreeTank = true; 
        maxBullets = 3;
        speed = 1.875;
        odds = 100;
    }
    bool getIsLevelThreeTank(){ return isLevelThreeTank; }

    void setLevelFourTank() {
        isLevelFourTank = true;
        maxBullets = 1;
        speed = 2.25;
    }
    bool getIsLevelFourTank(){ return isLevelFourTank; }
    std::vector<std::shared_ptr<Bomb>> m_bombVector{};

    void setLevelFiveTank() {
        isLevelFiveTank = true;
        maxBullets = 5;
        speed = 2.25;
        odds = 80;
    }
    bool getIsLevelFiveTank(){ return isLevelFiveTank; }

    // ai tank motions/member functions
    void rotateTurretAtPlayer(const Tank& player);

    void moveTowardsPlayer(const Tank& player);

    void plantBombEnemy(int maxBombs, Tank& player); 

    void getTankCoords() const;
    void test() const;

    // afterthought variables
    bool state1{ false };
    int everySecond{};
};

#endif
