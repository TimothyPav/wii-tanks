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

bool doOverlap(sf::RectangleShape s1, sf::RectangleShape s2) {
    return false;
}
