#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "bullet.h"
#include "wall.h"
#include "utils.h"


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

    // quadrant 1
    if (convertedAngle > 90 && convertedAngle <= 180) {
    }
    // quadrant 2
    if (convertedAngle >= 0 && convertedAngle <= 90) {
    } 
    // quadrant 3
    if (convertedAngle > 270) {
    }
    if (convertedAngle > 180 && convertedAngle <= 270) {
    }

    // std::cout << "Angle inside bullet constructor: " << convertedAngle << "  modAngle: " << modAngle << '\n';
    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}


sf::RectangleShape Bullet::getBody() {
    return body;
}

WallSide Bullet::whichSide(Wall& wall) {
    // 4 vectors left, right, top, bottom of body
    sf::Vector2f bodyLeft({body.getPosition().x, body.getPosition().y + (body.getSize().y/2)});
    sf::Vector2f bodyRight({body.getPosition().x + body.getSize().x, body.getPosition().y + (body.getSize().y/2)});
    sf::Vector2f bodyTop({body.getPosition().x + (body.getSize().x/2), body.getPosition().y});
    sf::Vector2f bodyBottom({body.getPosition().x + (body.getSize().x/2), body.getPosition().y + body.getSize().y});

    
    sf::RectangleShape wallBody = wall.getWall();
    sf::Vector2f wallLeft({wallBody.getPosition().x, wallBody.getPosition().y + (wallBody.getSize().y/2)});
    sf::Vector2f wallRight({wallBody.getPosition().x + wallBody.getSize().x, wallBody.getPosition().y + (wallBody.getSize().y/2)});
    sf::Vector2f wallTop({wallBody.getPosition().x + (wallBody.getSize().x/2), wallBody.getPosition().y});
    sf::Vector2f wallBottom({wallBody.getPosition().x + (wallBody.getSize().x/2), wallBody.getPosition().y + wallBody.getSize().y});


    if (contains(bodyLeft, wallBody)) {
        return WallSide::right;
    }
    else if (contains(bodyRight, wallBody)) {
        return WallSide::left;
    }
    else if (contains(bodyTop, wallBody)) {
        return WallSide::bottom;
    }
    else if (contains(bodyBottom, wallBody)) {
        return WallSide::top;
    }
    return WallSide::NODIRECTION;

}

bool Bullet::collision(sf::RenderWindow& window, std::vector<Wall>& level) {
    for (auto& wall : level) {
        if (doOverlap(wall.getWall(), body)) {
            // sf::Angle rotate = sf::degrees(180);
            WallSide side = whichSide(wall);
            switch (side) {
            case WallSide::right:
                angle = sf::degrees(180) - angle;
                std::cout << "WallSide::right\n";
                break;
            case WallSide::left:
                angle = sf::degrees(180) - angle;
                std::cout << "WallSide::left\n";
                break;
            case WallSide::bottom:
                angle = sf::degrees(180) - angle;
                break;
            case WallSide::top:
                angle = -angle;
                break;
            default: break;
            }
            return true;
        }
    }
    return false;
}

void Bullet::move(sf::RenderWindow& window, std::vector<Wall>& level) {
    
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

    collision(window, level);

    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}
