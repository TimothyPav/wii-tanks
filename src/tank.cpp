#include <iostream>
#include <SFML/Graphics.hpp>

#include "utils.h"
#include "wall.h"
#include "tank.h"

Tank::Tank(const sf::RectangleShape& body, const float& speed, const std::vector<Wall>& level)
    : body(body),
      speed(speed),
      level(level)
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
    if (dir == Direction::Up && checkBoundaries(dir)) {
        body.move({0.0f, -speed});
    }
    if (dir == Direction::Down && checkBoundaries(dir)) { // change magic numbers
        body.move({0.0f, speed});
    }
    if (dir == Direction::Left && checkBoundaries(dir)) {
        body.move({-speed, 0.0});
    }
    if (dir == Direction::Right && checkBoundaries(dir)) {
        body.move({speed, 0.0});
    }
}

// return true is tank is allowed to move there
bool Tank::checkBoundaries(Direction dir) {
    // tankTopLeftCoord is calculated as the POTENTIAL coordinate to check for bounds
    sf::Vector2f tankTopLeftCoord = sf::Vector2f(getX(), getY());
    if (dir == Direction::Up){
        tankTopLeftCoord.y -= speed;
    }
    else if (dir == Direction::Down){
        tankTopLeftCoord.y += speed;
    }
    else if (dir == Direction::Left){
        tankTopLeftCoord.x -= speed;
    }
    else if (dir == Direction::Right){
        tankTopLeftCoord.x += speed;
    }

    // offsets of 60 are there because they work i don't understand how it's magic. i can't figure out the math on this!
    sf::Vector2f tankBottomRightCoord = sf::Vector2f(tankTopLeftCoord.x+body.getSize().x+60, tankTopLeftCoord.y+body.getSize().y+60);
    sf::FloatRect windowBounds(sf::Vector2f(0.f, 0.f), sf::Vector2f(1920.0f, 1080.0f));
    if (!(windowBounds.contains(tankTopLeftCoord) && windowBounds.contains(tankBottomRightCoord))) {
        return false;
    }

    for (int i{0}; i < level.size(); ++i) {
        sf::RectangleShape wallObject = level[i].getWall();
        if (doOverlap(body, wallObject)){
            std::cout << "DO OVERLAP RETURNED TRUE\n";
        }
    }

    return true;
}

void Tank::test() const {
    std::cout << "hello from tank\n";
}
