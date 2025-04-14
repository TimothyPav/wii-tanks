#include <cstdlib>
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
    bool p = explosionTexture.loadFromFile("../assets/tank_explosion.png");
    if (!p) std::cout << "SOMETHING HORRIBLE HAS HAPPENED LOADING IN EXPLOSION TEXTURE\n";
}

Tank::Tank(const std::vector<Wall>& level, const float speed, const sf::Vector2f& position)
    : level(level)
    , speed(speed)
{
    body.setOrigin(sf::Vector2f(25, 25));
    body.setSize(sf::Vector2f(50, 50));
    body.setPosition(position);

    head.setSize(sf::Vector2f(30, 30));
    head.setPosition(sf::Vector2f(body.getPosition().x + 25, body.getPosition().y + 25)); // magic number 25 just works for centering the head
    sf::Vector2f headSize = head.getSize();
    head.setOrigin(sf::Vector2f({headSize.x / 2.0f, headSize.y / 2.0f}));

    turret.setSize(sf::Vector2f(50, 10));
    turret.setPosition(sf::Vector2f({body.getPosition().x+body.getSize().x/2.0f, body.getPosition().y+body.getSize().y/2.0f}));
    sf::Vector2f size = turret.getSize();
    turret.setOrigin(sf::Vector2f({size.x, size.y / 2.0f}));

    body.move(sf::Vector2f(25, 25));
    tankShapes.push_back(body);
    tankShapes.push_back(turret);

    bool p = explosionTexture.loadFromFile("../assets/tank_explosion.png");
    if (!p) std::cout << "SOMETHING HORRIBLE HAS HAPPENED LOADING IN EXPLOSION TEXTURE\n";
}

Tank::~Tank()
{
    if (isLevelFourTank)
    {
        for (auto bomb = m_bombVector.begin(); bomb != m_bombVector.end(); )
        {
            bomb = m_bombVector.erase(bomb);
        }
    }
}
int getOptimalRotationAngle(Direction dir, Direction dir2, int currentRotation); // decleration

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

void Tank::updateMoveValues(float xSpeed, float ySpeed) {
    body.move({xSpeed, ySpeed});
    turret.move({xSpeed, ySpeed});
    head.move({xSpeed, ySpeed});

    if (xSpeed != 0 && ySpeed != 0)
    {
        distance += std::floor(sqrt((xSpeed * xSpeed) + (ySpeed * ySpeed)));
    }
    else 
        distance += std::abs((xSpeed + ySpeed));
    // std::cout << distance << '\n';
    if (distance > 20)
    {
        sf::RectangleShape leftTread({body.getPosition()});
        sf::RectangleShape rightTread({body.getPosition()});
        leftTread.setOrigin({7.5, 7.5});
        leftTread.setRotation(body.getRotation());
        leftTread.setFillColor(sf::Color::Black);
        leftTread.setSize({15, 15});
        rightTread.setOrigin({7.5, 7.5});
        rightTread.setRotation(body.getRotation());
        rightTread.setFillColor(sf::Color::Black);
        rightTread.setSize({15, 15});

        if ((xSpeed > 0 && ySpeed > 0) || xSpeed < 0 && ySpeed < 0) // top left & bottom right
        {
            leftTread.setPosition({ body.getPosition().x - 24, body.getPosition().y });
            rightTread.setPosition({ body.getPosition().x, body.getPosition().y - 24 });
        }
        else if ((xSpeed > 0 && ySpeed < 0) || (xSpeed < 0 && ySpeed > 0)) // top right & bottom left
        {
            leftTread.setPosition({ body.getPosition().x, body.getPosition().y - 24 });
            rightTread.setPosition({ body.getPosition().x + 24, body.getPosition().y });
        }
        else if (ySpeed == 0.0) // left & right
        {
            leftTread.setPosition( {body.getPosition().x, body.getPosition().y - 18 });
            rightTread.setPosition( {body.getPosition().x, body.getPosition().y + 17 });
        }
        else // top & bottom
        {
            leftTread.setPosition( {body.getPosition().x - 17, body.getPosition().y });
            rightTread.setPosition( {body.getPosition().x + 18, body.getPosition().y });
        }
        treads.push_back(leftTread);
        treads.push_back(rightTread);
        
        distance = 0;
    }
}

void Tank::moveTank(Direction dir, Direction dir2) {
    // getTankCoords(); // debug print function
    // std::cout << "Directions from moveTank: " << static_cast<int>(dir) << ", " << static_cast<int>(dir2) << '\n';
        
    // std::cout << std::boolalpha << "checkBoundaries: " << checkBoundaries(dir, dir2) << '\n';

    if (!checkBoundaries(dir, dir2)) {
        if (!isPlayer)
            changeDir();
        return;
    }
    

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
    sf::Vector2f tankBombTopLeftCoord = sf::Vector2f(getX()-25, getY()-25);
    if (dir == Direction::Up || dir2 == Direction::Up){
        tankTopLeftCoord.y -= speed;
        tankBombTopLeftCoord.y -= (speed * (144 - getOptimalRotationAngle(dir, dir2, body.getRotation().asDegrees())));
    }
    if (dir == Direction::Down || dir2 == Direction::Down){
        tankTopLeftCoord.y += speed;
        tankBombTopLeftCoord.y += (speed * (144 - getOptimalRotationAngle(dir, dir2, body.getRotation().asDegrees())));
    }
    if (dir == Direction::Left || dir2 == Direction::Left){
        tankTopLeftCoord.x -= speed;
        tankBombTopLeftCoord.x -= (speed * (144 - getOptimalRotationAngle(dir, dir2, body.getRotation().asDegrees())));
    }
    if (dir == Direction::Right || dir2 == Direction::Right){
        tankTopLeftCoord.x += speed;
        tankBombTopLeftCoord.x += (speed * (144 - getOptimalRotationAngle(dir, dir2, body.getRotation().asDegrees())));
    }

    // offsets of 60 are there because they work i don't understand how it's magic. i can't figure out the math on this!
    sf::Vector2f tankBottomRightCoord = sf::Vector2f(tankTopLeftCoord.x+body.getSize().x, tankTopLeftCoord.y+body.getSize().y);
    sf::Vector2f tankBombBottomRightCoord = sf::Vector2f(tankBombTopLeftCoord.x+body.getSize().x, tankBombTopLeftCoord.y+body.getSize().y);
    sf::FloatRect windowBounds(sf::Vector2f(0.f, 0.f), sf::Vector2f(1920.0f-body.getSize().x-(speed*2), 1080.0f-body.getSize().y-(speed*2)));
    if (!(windowBounds.contains(tankTopLeftCoord) && windowBounds.contains(tankBottomRightCoord))) {
        return false;
    }


    for (int i{0}; i < currLevel.size(); ++i) {
        sf::RectangleShape wallObject = currLevel[i].getWall();
        // tankTopLeftCoord.x += (speed * 143);
        if (doOverlap(tankTopLeftCoord, tankBottomRightCoord, wallObject, speed)){
            return false;
        }
    }

    tankTopLeftCoord.x += 25;
    tankTopLeftCoord.y += 25;
    tankBombTopLeftCoord.x += 25;
    tankBombTopLeftCoord.y += 25;

    tankBottomRightCoord.x += 25;
    tankBottomRightCoord.y += 25;
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

    // work on this a little longer.
    if (!isPlayer)
    {
        for (const auto& bomb: bombs)
        {
            sf::CircleShape bombShape{ bomb->getBombBody() };
            bombShape.setRadius(100);
            if (doOverlap(tankBombTopLeftCoord, tankBombBottomRightCoord, bombShape, speed))
            {
                // std::cout << "tankBomb pred: " << ((tankBombTopLeftCoord.x + tankBottomRightCoord.x) / 2) << ", " 
                                               // << ((tankBombTopLeftCoord.y + tankBottomRightCoord.y) / 2) << '\n';
                // std::cout << "Actual bomb: " << bombShape.getPosition().x << ", " << bombShape.getPosition().y << '\n';
                changeDir();
            }
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

int getOptimalRotationAngle(Direction dir, Direction dir2, int currentRotation)
{
    if ((dir == Direction::Up && dir2 == Direction::Left) || (dir == Direction::Down && dir2 == Direction::Right)) {
        // Return the smaller of the two differences
        return (abs(currentRotation - 135) <= abs(currentRotation - 315)) ? 
               (currentRotation - 135) : (currentRotation - 315);
    }
    else if ((dir == Direction::Up && dir2 == Direction::Right) || (dir == Direction::Down && dir2 == Direction::Left)) {
        // Return the smaller of the two differences
        return (abs(currentRotation - 45) <= abs(currentRotation - 225)) ? 
               (currentRotation - 45) : (currentRotation - 225);
    }
    else if (dir2 == Direction::NODIRECTION && (dir == Direction::Left || dir == Direction::Right)) {
        // Return the smaller of the two differences
        return (abs(currentRotation - 0) <= abs(currentRotation - 180)) ? 
               (currentRotation - 0) : (currentRotation - 180);
    }
    else if (dir2 == Direction::NODIRECTION && (dir == Direction::Up || dir == Direction::Down)) {
        // Return the smaller of the two differences
        return (abs(currentRotation - 90) <= abs(currentRotation - 270)) ? 
               (currentRotation - 90) : (currentRotation - 270);
    }
    return 0; // Default: no rotation needed if no condition is met
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
    float dy= mousePos.y - objectCenter.y;

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
    if (currentBullets >= maxBullets) return;
    const float angleRadians = turret.getRotation().asRadians();
    float xSideLength = -std::cos(angleRadians) * 50;
    float ySideLength = -std::sin(angleRadians) * 50;

    auto newBullet = std::make_unique<Bullet>(
        turret.getPosition().x + xSideLength * 1.1, 
        turret.getPosition().y + ySideLength * 1.1, 
        2.25, 
        turret.getRotation(), 
        this
    );
    std::cout << "shoot bullet angle: " << turret.getRotation().asDegrees() << '\n';
    bullets.push_back(std::move(newBullet));
    ++currentBullets;
}

void Tank::plantBomb() {
    if (!isBombPlaced) {
        m_bomb = std::make_shared<Bomb>(body.getPosition().x, body.getPosition().y);
        isBombPlaced = true;
    }
}

void Tank::plantBombEnemy(int maxBombs, Tank& player) {
    sf::CircleShape potentialBomb{ sf::CircleShape(200) };
    potentialBomb.setPosition({body.getPosition().x - potentialBomb.getRadius(),
                               body.getPosition().y - potentialBomb.getRadius()});
    if (!contains(player.getTankBody().getPosition(), potentialBomb)) return;

    int randNum = Random::get(1, 150);
    if (m_bombVector.size() < maxBombs && randNum == 1)
    {
        std::shared_ptr<Bomb> b{ std::make_shared<Bomb>(body.getPosition().x, body.getPosition().y) };
        m_bombVector.push_back(b);
        bombs.push_back(b);
    }
}

void Tank::rotateTurretAtPlayer(const Tank& player) {
    float dx{ player.turret.getPosition().x - turret.getPosition().x };
    float dy{ player.turret.getPosition().y - turret.getPosition().y };

    float angle = std::atan2(dy, dx) * 180.0f / M_PI; 

    head.setRotation(sf::degrees(angle + 180));
    turret.setRotation(sf::degrees(angle + 180));

    const float dirX{ player.turret.getPosition().x - this->turret.getPosition().x };
    const float dirY{ player.turret.getPosition().y - this->turret.getPosition().y };
    const float distance{ (dirX * dirX) + (dirY * dirY) };
    const float normalizedX{ dirX / distance };
    const float normalizedY{ dirY / distance };
    const int checks{ 30 };
    const int stepSize{ static_cast<int>(distance / checks) };

    for (int i{0}; i <= checks; ++i)
    {
        const float pointX{ turret.getPosition().x + normalizedX * stepSize * i };
        const float pointY{ turret.getPosition().y + normalizedY * stepSize * i };

        const sf::Vector2f vec{ pointX, pointY };
        for (const auto& wall : currLevel)
        {
            if (!wall.isHole() && contains(vec, wall.getWall())) // DO NOT SHOOT IF BULLET WILL COLLIDE WITH WALL
            {
                return;
            }
        }
    }

    if (Random::get(1,odds) == 1) shoot();
}

void Tank::moveTowardsPlayer(const Tank& player) {
    float dx{ player.body.getPosition().x - body.getPosition().x };
    float dy{ player.body.getPosition().y - body.getPosition().y };

    float angle = std::atan2(dy, dx) * 180.0f / M_PI; 
    
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

void Tank::animate(float deltaTime)
{
    if (isAlive) return;

    if (!animation.playAnimation(0, deltaTime))
        animationFinished = true;

    std::cout << "Setting tank texture rect...\n";
    body.setTextureRect(animation.m_uvRect);
}

void Tank::getTankCoords() const {
    std::cout << "Tank body coords: (" << body.getPosition().x << ", " << body.getPosition().y << '\n';
}

void Tank::test() const {
    std::cout << "hello from tank\n";
}


