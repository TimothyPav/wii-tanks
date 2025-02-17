#include <iostream>

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

bool doOverlap(const sf::RectangleShape& s1, const sf::RectangleShape& s2) {
    sf::FloatRect rect1 = s1.getGlobalBounds();
    sf::FloatRect rect2 = s2.getGlobalBounds();
    
    return rect1.findIntersection(rect2) != std::nullopt;
}

bool contains(const sf::Vector2f point, const sf::RectangleShape rectangle) {
    sf::Vector2f topLeft({rectangle.getPosition().x, rectangle.getPosition().y});
    sf::Vector2f bottomRight({topLeft.x + rectangle.getSize().x, topLeft.y + rectangle.getSize().y});

    if (point.x > topLeft.x && point.x < bottomRight.x && point.y > topLeft.y && point.y < bottomRight.y)
        return true;
    return false;
}
