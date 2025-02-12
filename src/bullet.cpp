#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "bullet.h"

Bullet::Bullet(float x, float y, int speed, int angle) : speed(speed), angle(angle) {
    body.setPosition({x, y});
    body.setSize({10, 10});
    body.setFillColor(sf::Color::White);
    body.setRotation(sf::degrees(angle));
}


sf::RectangleShape Bullet::getBody() {
    return body;
}

void Bullet::move() {
    const int modAngle = angle % 90;

    float xSideLength = std::sin(modAngle) * speed;
    float ySideLength = std::cos(modAngle) * speed;
    
    // calculate which quadrant the bullet should be in
    if (angle >= 0 && angle <= 90) body.move({xSideLength, ySideLength});
    else if (angle > 90 && angle <= 180) body.move({-xSideLength, ySideLength});
    else if (angle > 180 && angle <= 270) body.move({-xSideLength, -ySideLength});
    else body.move({xSideLength, -ySideLength});

    // std::cout << "X side length: " << xSideLength << '\n';
    // std::cout << "Y side length: " << ySideLength << '\n';
}
