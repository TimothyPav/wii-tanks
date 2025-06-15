#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
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
    sf::Vector2f previousPosition{0, 0};

    sf::Texture explosionTexture;
    Animation animation = Animation(&explosionTexture, sf::Vector2u(6, 1), .05);
    bool animationFinished = false;

public:
    Bullet(float x, float y, float speed, sf::Angle angle, Tank* owner);
    
    sf::RectangleShape getBody();
    bool collision(sf::RenderWindow& window, std::vector<Wall>& level, 
                   std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<std::unique_ptr<Tank>>& tanks);
    WallSide whichSide(Wall& wall);

    int getBounces() { return bounces; }
    void setZeroBounces() { 
        sf::Vector2f centerPos{ body.getPosition() };
        body.setSize({30, 30});
        body.setOrigin({15, 15});
        body.setPosition(centerPos);

        body.setTexture(&explosionTexture);
        animation = Animation(&explosionTexture, sf::Vector2u(4, 1), .05);
        bounces = 0; 
    }

    void move(sf::RenderWindow& window, std::vector<Wall>& level, 
              std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<std::unique_ptr<Tank>>& tanks);
    Tank* decrementOwner(){ return owner; }
    

    Animation& getAnimation(){ return animation; }
    void animate(float deltaTime);
    bool isAnimationFinished(){ return animationFinished; }
};


#endif
