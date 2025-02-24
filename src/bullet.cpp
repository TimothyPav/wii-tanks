#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <limits>

#include "bullet.h"
#include "wall.h"
#include "utils.h"
#include "tank.h"
#include <unistd.h>


Bullet::Bullet(float x, float y, int speed, sf::Angle angle) : speed(speed), angle(angle) {
    body.setOrigin({5, 5});
    body.setRotation(angle);
    body.setPosition({x, y});
    body.setSize({10, 10});
    body.setFillColor(sf::Color::White);

    const float convertedAngle = angle.asDegrees();
    const float angleRadians = angle.asRadians();

    // (adjacent side): x = c * cos(θ)
    // (opposite side): y = c * sin(θ)

    // std::cout << "Angle inside bullet constructor: " << convertedAngle << "  modAngle: " << modAngle << '\n';
    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}


sf::RectangleShape Bullet::getBody() {
    return body;
}

WallSide Bullet::whichSide(Wall& wall) {
    // top-left, top-right, bottom-left, bottom-right
    std::array<sf::Vector2f, 4> bodies {};
    
    bodies[0] = sf::Vector2f ({body.getPosition().x, body.getPosition().y});
    bodies[1] = sf::Vector2f ({body.getPosition().x+body.getSize().x, body.getPosition().y});
    bodies[2] = sf::Vector2f ({body.getPosition().x, body.getPosition().y+body.getSize().y});
    bodies[3] = sf::Vector2f ({body.getPosition().x+body.getSize().x, body.getPosition().y+body.getSize().y});

    std::array<float, 4> wallCoords {};

    sf::RectangleShape recWall = wall.getWall();
    wallCoords[0] = float { recWall.getPosition().x };
    wallCoords[1] = float { recWall.getPosition().x+recWall.getSize().x };
    wallCoords[2] = float { recWall.getPosition().y };
    wallCoords[3] = float { recWall.getPosition().y+recWall.getSize().y };

    double positiveInfinity = std::numeric_limits<double>::infinity();
    double min{ positiveInfinity };
    double calc { 0 };
    WallSide dir { WallSide::NODIRECTION };

    for (int i{0}; i < bodies.size(); ++i) {
        for (int j{0}; j < wallCoords.size(); ++j) {
            sf::Vector2f bodiesI { bodies[i] };
            float wallCoord { wallCoords[j] };

            if (j < 2) {
                // if (j == 0) std::cout << "Left wall X: ";
                // if (j == 1) std::cout << "Right wall X: ";
                // std::cout << bodiesI.x - wallCoord << '\n';
                calc = static_cast<double>(bodiesI.x - wallCoord);
            } else {
                // if (j == 2) std::cout << "Top wall Y: ";
                // if (j == 3) std::cout << "Bottom wall Y: ";
                // std::cout << bodiesI.y - wallCoord << '\n';
                calc = static_cast<double>(bodiesI.y - wallCoord);
            }
            calc = std::abs(calc);
            if (calc < min) {
                min = calc;
                dir = static_cast<WallSide>(j+1);
            }
            // std::cout << bodiesI.y - wallCoord << '\n';
        }
    }
    switch (dir) {
    case WallSide::right:
        std::cout << "Right Side\n";
        break;
    case WallSide::left:
        std::cout << "Left Side\n";
        break;
    case WallSide::bottom:
        std::cout << "Bottom Side\n";
        break;
    case WallSide::top:
        std::cout << "Top Side\n";
        break;
    default: break;
    }
    std::cout << '\n';

    return dir;
}

bool Bullet::collision(sf::RenderWindow& window, std::vector<Wall>& level, std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<std::unique_ptr<Tank>>& tanks) {
    for (auto& wall : level) {
        if (doOverlap(wall.getWall(), body)) {
            // sf::Angle rotate = sf::degrees(180);
            WallSide side = whichSide(wall);
            switch (side) {
            case WallSide::right:
                angle = sf::degrees(180) - angle;
                break;
            case WallSide::left:
                angle = sf::degrees(180) - angle;
                break;
            case WallSide::bottom:
                angle = -angle;
                break;
            case WallSide::top:
                angle = -angle;
                break;
            default: break;
            }
            return true;
        }
    }
    if (body.getPosition().x <= 5 || body.getPosition().x >= 1850) {
        angle = sf::degrees(180) - angle;
        return true;
    }
    if (body.getPosition().y <= 5 || body.getPosition().y >= 1010) {
        angle = -angle;
        return true;
    }

    for (const auto& bomb : bombs) {
       if (doOverlap((*bomb).getBombBody(), body)) {
           bomb->explode(tanks);
       } 
    }

    for (const auto& tank : tanks) {
        if (doOverlap(tank->getTankBody(), body)) {
            tank->kill();
            bounces = 0;
        }
    }

    return false;
}

void Bullet::move(sf::RenderWindow& window, std::vector<Wall>& level, std::vector<std::shared_ptr<Bomb>>& bombs, std::vector<std::unique_ptr<Tank>>& tanks) {
    
    if (bounces <= 0) return;
    if (collision(window, level, bombs, tanks)) --bounces;
    const float convertedAngle = angle.asDegrees();
    const float angleRadians = angle.asRadians();
    const float speedF = static_cast<float>(speed);

    float xSideLength = -std::cos(angleRadians) * speed;
    float ySideLength = -std::sin(angleRadians) * speed;
    
    // calculate which quadrant the bullet should be in
    if (convertedAngle >= 0 && convertedAngle <= 90) body.move({xSideLength, ySideLength});
    else if (convertedAngle > 90 && convertedAngle <= 180) body.move({xSideLength, ySideLength});
    else if (convertedAngle > 180 && convertedAngle <= 270) body.move({xSideLength, ySideLength});
    else body.move({xSideLength, ySideLength});


    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}
