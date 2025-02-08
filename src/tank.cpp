#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

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

std::vector<sf::RectangleShape> Tank::getBody() {
    return tankShapes;
}

sf::RectangleShape Tank::getTankBody() {
    return body;
}

sf::RectangleShape Tank::getTurretBody() {
    return turret;
}

sf::RectangleShape Tank::getHeadBody() {
    return head;
}

float Tank::getX() {
    return body.getPosition().x;
}

float Tank::getY() {
    return body.getPosition().y;
}

void Tank::moveTank(Direction dir) {
    checkRotation(dir);
    // getTankCoords(); // debug print function
    if (dir == Direction::Up && checkBoundaries(dir)) {
        body.move({0.0f, -speed});
        turret.move({0.0f, -speed});
        head.move({0.0f, -speed});
    }
    if (dir == Direction::Down && checkBoundaries(dir)) { // change magic numbers
        body.move({0.0f, speed});
        turret.move({0.0f, speed});
        head.move({0.0f, speed});
    }
    if (dir == Direction::Left && checkBoundaries(dir)) {
        body.move({-speed, 0.0});
        turret.move({-speed, 0.0});
        head.move({-speed, 0.0});
    }
    if (dir == Direction::Right && checkBoundaries(dir)) {
        body.move({speed, 0.0});
        turret.move({speed, 0.0});
        head.move({speed, 0.0});
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
    sf::Vector2f tankBottomRightCoord = sf::Vector2f(tankTopLeftCoord.x+body.getSize().x, tankTopLeftCoord.y+body.getSize().y);
    sf::FloatRect windowBounds(sf::Vector2f(0.f, 0.f), sf::Vector2f(1920.0f-body.getSize().x-(speed*2), 1080.0f-body.getSize().y-(speed*2)));
    if (!(windowBounds.contains(tankTopLeftCoord) && windowBounds.contains(tankBottomRightCoord))) {
        return false;
    }


    for (int i{0}; i < level.size(); ++i) {
        sf::RectangleShape wallObject = level[i].getWall();
        if (doOverlap(tankTopLeftCoord, tankBottomRightCoord, wallObject, speed)){
            return false;
        }
    }

    return true;
}

bool Tank::checkRotation(Direction dir) {
    if (dir == Direction::Right && (body.getRotation() == sf::degrees(0.0f) || body.getRotation() == sf::degrees(180.0f))) {
    } else {
        body.rotate(body.getRotation() + sf::degrees(0.0001)); // fix this :(
        std::cout << "Body angle: " << body.getRotation().asDegrees() << '\n';
    }
    return false;
}

void Tank::rotateTurretBasedOnMouse(sf::Vector2i mousePos) {
    sf::Vector2f objectCenter = turret.getPosition();
    sf::Vector2f size = turret.getSize();
    turret.setOrigin(sf::Vector2f({size.x, size.y / 2.0f}));

    sf::Vector2f headObjectCenter = head.getPosition();
    sf::Vector2f headSize = head.getSize();

    // sf::Vector2f sizeOfHead = head.getSize();
    head.setOrigin(sf::Vector2f({head.getSize().x / 2.0f, head.getSize().y / 2.0f}));
    // std::cout << "Turrent center: " << objectCenter.x << ", " << objectCenter.y << '\n';
    // std::cout << "Mouse center: " << mousePos.x << ", " << mousePos.y << "  ORIGIN OF SHAPE: " << turret.getOrigin().x << ", " << turret.getOrigin().y << '\n';

    float dx = mousePos.x - objectCenter.x;
    float dy = mousePos.y - objectCenter.y;

    float angle = std::atan2(dy, dx) * 180.0f / M_PI; 
    turret.setRotation(sf::degrees(angle + 180)); // Convert to degrees

    // dx = mousePos.x - head.getPosition().x;
    // dy = mousePos.y - head.getPosition().y;
//
    // angle = std::atan2(dy, dx) * 180.0f / M_PI; 
    head.setRotation(sf::degrees(angle + 180));
}


void Tank::getTankCoords() const {
    std::cout << "Tank body coords: (" << body.getPosition().x << ", " << body.getPosition().y << '\n';
}

void Tank::test() const {
    std::cout << "hello from tank\n";
}
