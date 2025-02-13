#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "bullet.h"

Bullet::Bullet(float x, float y, int speed, sf::Angle angle) : speed(speed), angle(angle) {
    body.setPosition({x, y});
    body.setSize({10, 10});
    body.setFillColor(sf::Color::White);
    // body.setRotation(angle);

    const int modAngle = static_cast<int>(angle.asDegrees()) % 90;
    const float convertedAngle = angle.asDegrees();

    // (adjacent side): x = c * cos(θ)
    // (opposite side): y = c * sin(θ)

    // quadrant 1
    if (convertedAngle > 90 && convertedAngle <= 180) {
        float xSideLength = std::cos(convertedAngle) * speed;
        float ySideLength = std::sin(convertedAngle) * speed;
        std::cout << "X side length: " << xSideLength << '\n'; // i have no idea
        std::cout << "Y side length: " << ySideLength << '\n';
    }
    if (convertedAngle >= 0 && convertedAngle <= 90) std::cout << "Quadrant 2\n";
    if (convertedAngle > 270)  std::cout << "Quadrant 3\n";
    if (convertedAngle > 180 && convertedAngle <= 270) std::cout << "Quadrant 4\n";

    // std::cout << "Angle inside bullet constructor: " << convertedAngle << "  modAngle: " << modAngle << '\n';
    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}


sf::RectangleShape Bullet::getBody() {
    return body;
}

void Bullet::move() {
    
    const int modAngle = static_cast<int>(angle.asDegrees()) % 90;
    const float convertedAngle = angle.asDegrees();
    const float speedF = static_cast<float>(speed);

    // float xSideLength = std::sin(modAngle) * speed;
    // float ySideLength = std::cos(modAngle) * speed;
    
    // calculate which quadrant the bullet should be in
    if (convertedAngle >= 0 && convertedAngle <= 90) body.move({-speedF, 0});
    else if (convertedAngle > 90 && convertedAngle <= 180) body.move({speedF, 0});
    else if (convertedAngle > 180 && convertedAngle <= 270) body.move({speedF, 0});
    else body.move({-speedF, 0});

    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}
