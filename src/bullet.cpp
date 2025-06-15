#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Angle.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <unistd.h>

#include "bullet.h"
#include "wall.h"
#include "utils.h"
#include "tank.h"

extern std::vector<std::unique_ptr<Bullet>> bullets;

Bullet::Bullet(float x, float y, float speed, sf::Angle angle, Tank* owner) : speed(speed), angle(angle), owner(owner) {
    body.setOrigin({5, 5});
    body.setRotation(angle);
    body.setPosition({x, y});
    body.setSize({20, 10});
    body.setFillColor(sf::Color::White);

    const float convertedAngle = angle.asDegrees();
    const float angleRadians = angle.asRadians();

    bool p = explosionTexture.loadFromFile("../assets/bubble.png");
    // (adjacent side): x = c * cos(θ)
    // (opposite side): y = c * sin(θ)

    // std::cout << "Angle inside bullet constructor: " << convertedAngle << "  modAngle: " << modAngle << '\n';
    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}


sf::RectangleShape Bullet::getBody() {
    return body;
}

void Bullet::animate(float deltaTime)
{
    if (!animation.playAnimation(0, deltaTime))
        animationFinished = true;
    body.setTextureRect(animation.m_uvRect);
}

WallSide Bullet::whichSide(Wall& wall) {
    sf::FloatRect bulletBounds = body.getGlobalBounds();
    sf::FloatRect wallBounds = wall.getWall().getGlobalBounds();
    
    sf::Vector2f bulletCenter = bulletBounds.getCenter();
    sf::Vector2f wallCenter = wallBounds.getCenter();
    
    // Get velocity vector from previous position
    sf::Vector2f velocity = body.getPosition() - previousPosition;
    
    // Direction from wall to bullet
    sf::Vector2f direction = bulletCenter - wallCenter;
    
    // Calculate penetration depths
    float penetrationX = (bulletBounds.size.x + wallBounds.size.x) / 2 - std::abs(direction.x);
    float penetrationY = (bulletBounds.size.y + wallBounds.size.y) / 2 - std::abs(direction.y);
    
    // For corners, consider the movement direction
    if (fabs(penetrationX - penetrationY) < 5.0f) {
        // It's a corner collision - use velocity to determine side
        if (fabs(velocity.x) > fabs(velocity.y)) {
            return (velocity.x > 0) ? WallSide::left : WallSide::right;
        } else {
            return (velocity.y > 0) ? WallSide::top : WallSide::bottom;
        }
    }
    
    // For clear side collisions
    if (penetrationX < penetrationY) {
        return (direction.x > 0) ? WallSide::right : WallSide::left;
    } else {
        return (direction.y > 0) ? WallSide::bottom : WallSide::top;
    }
}

bool Bullet::collision(sf::RenderWindow& window, std::vector<Wall>& level, std::vector<std::shared_ptr<Bomb>>& bombs, 
                       std::vector<std::unique_ptr<Tank>>& tanks) {
    for (auto& wall : level) {
        if (!wall.isHole() && doOverlap2(wall.getWall(), body)) {
            // sf::Angle rotate = sf::degrees(180);
            WallSide side = whichSide(wall);
            switch (side) {
            case WallSide::right:
                angle = sf::degrees(180) - angle;
                body.move({10, 0});
                break;
            case WallSide::left:
                angle = sf::degrees(180) - angle;
                body.move({-10, 0});
                break;
            case WallSide::bottom:
                angle = -angle;
                body.move({0, 10});
                break;
            case WallSide::top:
                angle = -angle;
                body.move({0, -10});
                break;
            default: break;
            }
            body.setRotation(angle);
            return true;
        }
    }
    if (body.getPosition().x <= 5 || body.getPosition().x >= 1850) {
        angle = sf::degrees(180) - angle;
        body.setRotation(angle);
        return true;
    }
    if (body.getPosition().y <= 5 || body.getPosition().y >= 1010) {
        angle = -angle;
        body.setRotation(angle);
        return true;
    }

    for (const auto& bomb : bombs) {
       if (bomb->getIsActive() && doOverlap((*bomb).getBombBody(), body)) {
           bomb->explode(tanks);
           bounces = 0;
       } 
    }


    for (const auto& tank : tanks) {
        if (tank->getIsAlive() && doOverlap2(tank->getTankBody(), body)) {
            tank->kill();
            bounces = 0;
        }
    }

    return false;
}

void Bullet::move(sf::RenderWindow& window, std::vector<Wall>& level, std::vector<std::shared_ptr<Bomb>>& bombs, 
                  std::vector<std::unique_ptr<Tank>>& tanks) {
    
    if (collision(window, level, bombs, tanks)) --bounces;
    const float convertedAngle = angle.asDegrees();
    const float angleRadians = angle.asRadians();
    const float speedF = static_cast<float>(speed);

    float xSideLength = -std::cos(angleRadians) * speed;
    float ySideLength = -std::sin(angleRadians) * speed;
    
    // calculate which quadrant the bullet should be in
    previousPosition = body.getPosition();
    if (convertedAngle >= 0 && convertedAngle <= 90) body.move({xSideLength, ySideLength});
    else if (convertedAngle > 90 && convertedAngle <= 180) body.move({xSideLength, ySideLength});
    else if (convertedAngle > 180 && convertedAngle <= 270) body.move({xSideLength, ySideLength});
    else body.move({xSideLength, ySideLength});

    if (bounces <= 0) {
        setZeroBounces();
        return;
    }
    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}
