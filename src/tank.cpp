#include <iostream>
#include <SFML/Graphics.hpp>

#include "tank.h"

Tank::Tank(const sf::RectangleShape& body, const float& speed)
    : body(body),
      speed(speed)
{
    setDefaults();
}

sf::RectangleShape& Tank::getBody() {
    return body;
}

float Tank::getX() {
    return body.getPosition().x;
}

float Tank::getY() {
    return body.getPosition().y;
}

void Tank::moveTank(Direction dir) {
    std::cout << "Position of Tank: " << getX() << ", " << getY() << '\n';
    if (dir == Direction::Up && getY() > 0.0) {
        body.move({0.0f, -speed});
    }
    if (dir == Direction::Down && getY() < 960) { // change magic numbers
        body.move({0.0f, speed});
    }
    if (dir == Direction::Left) {
        body.move({-speed, 0.0});
    }
    if (dir == Direction::Right) {
        body.move({speed, 0.0});
    }
}

void Tank::test() const {
    std::cout << "hello from tank\n";
}
