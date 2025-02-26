#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "utils.h"
#include "wall.h"
#include "tank.h"
#include "Random.h"

Tank::Tank(const sf::RectangleShape& body, const float& speed, const std::vector<Wall>& level)
    : body(body),
      speed(speed),
      level(level)
{
    setDefaults();
}

Tank::Tank(const std::vector<Wall>& level, const float speed, const sf::Vector2f& position)
    : level(level)
    , speed(speed)
{
    body.setOrigin(sf::Vector2f(25, 25));
    body.setSize(sf::Vector2f(50, 50));
    body.setFillColor(sf::Color::Green);
    body.setPosition(position);

    head.setSize(sf::Vector2f(30, 30));
    head.setPosition(sf::Vector2f(body.getPosition().x + 25, body.getPosition().y + 25)); // magic number 25 just works for centering the head
    head.setFillColor(sf::Color::White);
    sf::Vector2f headSize = head.getSize();
    head.setOrigin(sf::Vector2f({headSize.x / 2.0f, headSize.y / 2.0f}));

    turret.setSize(sf::Vector2f(50, 10));
    turret.setPosition(sf::Vector2f({body.getPosition().x+body.getSize().x/2.0f, body.getPosition().y+body.getSize().y/2.0f}));
    turret.setFillColor(sf::Color::White);
    sf::Vector2f size = turret.getSize();
    turret.setOrigin(sf::Vector2f({size.x, size.y / 2.0f}));

    body.move(sf::Vector2f(25, 25));
    std::cout << "constructor called for enemy tank\n";
    tankShapes.push_back(body);
    tankShapes.push_back(turret);
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

std::vector<Bullet>& Tank::getBulletSet() {
    return bullets;
}

void Tank::updateMoveValues(float xSpeed, float ySpeed) {
    body.move({xSpeed, ySpeed});
    turret.move({xSpeed, ySpeed});
    head.move({xSpeed, ySpeed});
}

void Tank::moveTank(Direction dir, Direction dir2) {
    // getTankCoords(); // debug print function
    // std::cout << "Directions from moveTank: " << static_cast<int>(dir) << ", " << static_cast<int>(dir2) << '\n';
        
    // std::cout << std::boolalpha << "checkBoundaries: " << checkBoundaries(dir, dir2) << '\n';

    if (!checkBoundaries(dir, dir2)) return;
    if (!checkRotation(dir, dir2)) return; // function does something outside its return value

    if (dir == Direction::Up && dir2 == Direction::Left) {
        updateMoveValues(-speed, -speed);
    }
    else if (dir == Direction::Up && dir2 == Direction::Right) {
        updateMoveValues(speed, -speed);
    }
    else if (dir == Direction::Down && dir2 == Direction::Right) {
        updateMoveValues(speed, speed);
    }
    else if (dir == Direction::Down && dir2 == Direction::Left) {
        updateMoveValues(-speed, speed);
    }
    else if (dir == Direction::Up) {
        updateMoveValues(0.0, -speed);
    }
    else if (dir == Direction::Down) {
        updateMoveValues(0.0, speed);
    }
    else if (dir == Direction::Left) {
        updateMoveValues(-speed, 0.0);
    }
    else if (dir == Direction::Right) {
        updateMoveValues(speed, 0.0);
    }
}

// return true is tank is allowed to move there
bool Tank::checkBoundaries(Direction dir, Direction dir2) {
    // tankTopLeftCoord is calculated as the POTENTIAL coordinate to check for bounds
    sf::Vector2f tankTopLeftCoord = sf::Vector2f(getX()-25, getY()-25);
    if (dir == Direction::Up || dir2 == Direction::Up){
        tankTopLeftCoord.y -= speed;
    }
    if (dir == Direction::Down || dir2 == Direction::Down){
        tankTopLeftCoord.y += speed;
    }
    if (dir == Direction::Left || dir2 == Direction::Left){
        tankTopLeftCoord.x -= speed;
    }
    if (dir == Direction::Right || dir2 == Direction::Right){
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

    tankTopLeftCoord.x += 25;
    tankTopLeftCoord.y += 25;

    tankBottomRightCoord.x += 25;
    tankBottomRightCoord.y += 25;
    // make this work better

    for (const auto& currentTank : tanks)
    {
        if (this != currentTank.get() && doOverlap(tankTopLeftCoord, tankBottomRightCoord, currentTank->body, speed))
        {
            return false;
        }
    }

    return true;
}

int getOptimalRotationHelper(int desiredRotation, int currentRotation) {
    if (currentRotation - desiredRotation < 0) return 1;
    else return -1;
}

int getOptimalRotation(Direction dir, Direction dir2, int currentRotation) {
    if ((dir == Direction::Up && dir2 == Direction::Left) || (dir == Direction::Down && dir2 == Direction::Right)) {
        // condition ? expression1 : expression2
        return (abs(currentRotation - 135) <= abs(currentRotation - 315)) ? getOptimalRotationHelper(135, currentRotation) : getOptimalRotationHelper(315, currentRotation);
    }
    else if ((dir == Direction::Up && dir2 == Direction::Right) || (dir == Direction::Down && dir2 == Direction::Left)) {
        // condition ? expression1 : expression2
        return (abs(currentRotation - 45) <= abs(currentRotation - 225)) ? getOptimalRotationHelper(45, currentRotation) : getOptimalRotationHelper(225, currentRotation);
    }
    else if (dir2 == Direction::NODIRECTION && (dir == Direction::Left || dir == Direction::Right)) {
        return (abs(currentRotation - 0) <= abs(currentRotation - 180)) ? getOptimalRotationHelper(0, currentRotation) : getOptimalRotationHelper(180, currentRotation);
    }
    else if (dir2 == Direction::NODIRECTION && (dir == Direction::Up || dir == Direction::Down)) {
        return (abs(currentRotation - 90) <= abs(currentRotation - 270)) ? getOptimalRotationHelper(90, currentRotation) : getOptimalRotationHelper(270, currentRotation);
    }
    return 1;
}

bool Tank::checkRotation(Direction dir, Direction dir2) {
    // angle %= 360;
    const int currentRotation = std::floor(body.getRotation().asDegrees());

    // std::cout << "Current angle: " << currentRotation << "  Directions: " << static_cast<int>(dir) << ", " << static_cast<int>(dir2) << '\n';
    if ((dir == Direction::Up && dir2 == Direction::Left || dir == Direction::Down && dir2 == Direction::Right) && (currentRotation == 135 || currentRotation == 315)) {
        return true;
    }
    else if((dir == Direction::Up && dir2 == Direction::Right || dir == Direction::Down && dir2 == Direction::Left) && (currentRotation == 45 || currentRotation == 225)) {
        return true;
    }
    else if (((dir == Direction::Left || dir == Direction::Right) && dir2 == Direction::NODIRECTION) && (currentRotation == 0 || currentRotation == 180)) {
        return true;
    } 
    else if (((dir == Direction::Up || dir == Direction::Down) && dir2 == Direction::NODIRECTION) && (currentRotation == 90 || currentRotation == 270)) {
        return true;
    }

    int optimalDirection { getOptimalRotation(dir, dir2, currentRotation) };
    body.rotate(sf::degrees(optimalDirection));
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


void Tank::shoot() {
    // const int angle { static_cast<int>(std::floor(shape.getRotation().asDegrees())) }; // need to rotate this by 180 degrees
    // std::cout << "angle of turret: " << turret.getRotation().asDegrees() << '\n';
    if (bullets.size() >= maxBullets) return;
    std::cout << "shoot called\n";
    const float angleRadians = turret.getRotation().asRadians();
    float xSideLength = -std::cos(angleRadians) * 50;
    float ySideLength = -std::sin(angleRadians) * 50;

    Bullet bullet(turret.getPosition().x+xSideLength, turret.getPosition().y+ySideLength, 3, turret.getRotation());
    bullets.push_back(bullet);
}

void Tank::plantBomb() {
    if (!isBombPlaced) {
        std::cout << "Plant bomb is called meaning that you can plant another bomb\n";
        m_bomb = std::make_shared<Bomb>(body.getPosition().x, body.getPosition().y);
        isBombPlaced = true;
    }
}

void Tank::rotateTurretAtPlayer(const Tank& player) {
    float dx{ player.body.getPosition().x - body.getPosition().x };
    float dy{ player.body.getPosition().y - body.getPosition().y };

    float angle = std::atan2(dy, dx) * 180.0f / M_PI; 

    head.setRotation(sf::degrees(angle + 180));
    turret.setRotation(sf::degrees(angle + 180));


    if (Random::get(1, 300) == 1) shoot();
}

void Tank::moveTowardsPlayer(const Tank& player) {
    float dx{ player.body.getPosition().x - body.getPosition().x };
    float dy{ player.body.getPosition().y - body.getPosition().y };

    float angle = std::atan2(dy, dx) * 180.0f / M_PI; 
    std::cout << "angle: " << angle << '\n';
    
    // fix bug where he starts tweakin

    if (angle > -22.5 && angle < 22.5) dir = std::pair (Direction::Right, Direction::NODIRECTION); // CTAD
    else if (angle >= 22.5 && angle < 67.5) dir = std::pair (Direction::Down, Direction::Right);
    else if (angle >= 67.5 && angle < 112.5) dir = std::pair (Direction::Down, Direction::NODIRECTION);
    else if (angle >= 112.5 && angle < 157.5) dir = std::pair (Direction::Down, Direction::Left);
    else if (angle >= 157.5 || angle < -157.5) dir = std::pair (Direction::Left, Direction::NODIRECTION);
    else if (angle >= -157.5 && angle < -112.5) dir = std::pair (Direction::Up, Direction::Left);
    else if (angle >= -112.5 && angle < -67.5) dir = std::pair (Direction::Up, Direction::NODIRECTION);
    else if (angle >= -67.5 && angle <= -22.5) dir = std::pair (Direction::Up, Direction::Right);

}

void Tank::getTankCoords() const {
    std::cout << "Tank body coords: (" << body.getPosition().x << ", " << body.getPosition().y << '\n';
}

void Tank::test() const {
    std::cout << "hello from tank\n";
}
