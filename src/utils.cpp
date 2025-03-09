#include <iostream>
#include <utility>

#include "utils.h"

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::RectangleShape s2, int speed)
{
    sf::Vector2f pos2 = s2.getPosition();
    
    // Get size of each rectangle 
    sf::Vector2f size2 = (s2.getSize());
    
    // Calculate bottom-right corners
    sf::Vector2f bottomRight2(pos2.x + size2.x, pos2.y + size2.y);

    // std::cout << "Tank: " << tankTopLeft.x << "," << tankTopLeft.y << " to " << tankBottomRight.x << "," << tankBottomRight.y << "\n";
    // std::cout << "Rectangle 2: " << pos2.x << "," << pos2.y << " to " << bottomRight2.x << "," << bottomRight2.y << "\n";
    // std::cout << '\n';
    // 

    if (tankTopLeft.x > bottomRight2.x-speed || pos2.x+speed > tankBottomRight.x)
        return false;
    if (tankTopLeft.y > bottomRight2.y-speed || pos2.y+speed > tankBottomRight.y)
        return false;
    return true;
}

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::CircleShape circle, int speed)
{
    // Get circle position (which is the center in SFML CircleShape)
    sf::Vector2f circleCenter = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
    
    // Get circle radius
    float radius = circle.getRadius();
    
    // Find the closest point on the rectangle to the circle
    float closestX = std::max(tankTopLeft.x, std::min(circleCenter.x, tankBottomRight.x));
    float closestY = std::max(tankTopLeft.y, std::min(circleCenter.y, tankBottomRight.y));
    
    // Calculate the distance between the closest point and the circle center
    float distanceX = circleCenter.x - closestX;
    float distanceY = circleCenter.y - closestY;
    
    // If the distance is less than the radius, the circle and rectangle overlap
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    
    // Account for the speed parameter by effectively increasing the circle's radius
    // This creates a buffer zone around the circle
    return distanceSquared <= radius  * radius;
}

bool doOverlap(const sf::RectangleShape& s1, const sf::RectangleShape& s2) {
    sf::FloatRect rect1 = s1.getGlobalBounds();
    sf::FloatRect rect2 = s2.getGlobalBounds();
    
    return rect1.findIntersection(rect2) != std::nullopt;
}


bool doOverlap(const sf::CircleShape& s1, const sf::RectangleShape& s2) {
    sf::FloatRect circle = s1.getGlobalBounds();
    sf::FloatRect rect = s2.getGlobalBounds();
    
    return circle.findIntersection(rect) != std::nullopt;
}

bool contains(const sf::Vector2f point, const sf::RectangleShape rectangle) {
    sf::Vector2f topLeft({rectangle.getPosition().x, rectangle.getPosition().y});
    sf::Vector2f bottomRight({topLeft.x + rectangle.getSize().x, topLeft.y + rectangle.getSize().y});

    if (point.x > topLeft.x && point.x < bottomRight.x && point.y > topLeft.y && point.y < bottomRight.y)
        return true;
    return false;
}

std::pair<Direction, Direction> getDirection(int choice)
{
    switch(choice)
    {
    // Uses Class Template Argument Deduction
    case 0: return std::pair { Direction::Up, Direction::Left };
    case 1: return std::pair { Direction::Up, Direction::Right };
    case 2: return std::pair { Direction::Down, Direction::Right };
    case 3: return std::pair { Direction::Down, Direction::Left };
    case 4: return std::pair { Direction::Up, Direction::NODIRECTION };
    case 5: return std::pair { Direction::Down, Direction::NODIRECTION };
    case 6: return std::pair { Direction::Left, Direction::NODIRECTION };
    case 7: return std::pair { Direction::Right, Direction::NODIRECTION };
    default: return std::pair { Direction::Up, Direction::Left };
    }
}
