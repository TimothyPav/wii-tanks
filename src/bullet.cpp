#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "bullet.h"

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
        float xSideLength = -std::cos(angleRadians) * speed;
        float ySideLength = -std::sin(angleRadians) * speed;
        std::cout << "X side length: " << xSideLength << '\n'; // i have no idea
        std::cout << "Y side length: " << ySideLength << '\n';
    }
    // quadrant 2
    if (convertedAngle >= 0 && convertedAngle <= 90) {
        float xSideLength = -std::cos(angleRadians) * speed;
        float ySideLength = -std::sin(angleRadians) * speed;
        std::cout << "X side length: " << xSideLength << '\n'; // i have no idea
        std::cout << "Y side length: " << ySideLength << '\n';
    } 
    // quadrant 3
    if (convertedAngle > 270) {
        float xSideLength = -std::cos(angleRadians) * speed;
        float ySideLength = -std::sin(angleRadians) * speed;
        std::cout << "X side length: " << xSideLength << '\n'; // i have no idea
        std::cout << "Y side length: " << ySideLength << '\n';
    }
    if (convertedAngle > 180 && convertedAngle <= 270) {
        float xSideLength = -std::cos(angleRadians) * speed;
        float ySideLength = -std::sin(angleRadians) * speed;
        std::cout << "X side length: " << xSideLength << '\n'; // i have no idea
        std::cout << "Y side length: " << ySideLength << '\n';
    }

    // std::cout << "Angle inside bullet constructor: " << convertedAngle << "  modAngle: " << modAngle << '\n';
    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}


sf::RectangleShape Bullet::getBody() {
    return body;
}

void Bullet::move() {
    
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
