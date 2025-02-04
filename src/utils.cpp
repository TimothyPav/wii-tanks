#include <iostream>

#include "utils.h"

bool doOverlap(sf::RectangleShape s1, sf::RectangleShape s2)
{
    // l1 is top left
    // l2 is bottom right
    sf::Vector2f l1 = s1.getPoint(0);
    sf::Vector2f l2 = s1.getPoint(3);

    sf::Vector2f r1 = s2.getPoint(0);
    sf::Vector2f r2 = s2.getPoint(3);

    std::cout << s1.getOrigin().x << '\n';

    // std::cout << l1.x << ", " << l1.y << '\n';
    // std::cout << l2.x << ", " << l2.y << '\n';
//
    // std::cout << r1.x << ", " << r1.y << '\n';
    // std::cout << r2.x << ", " << r2.y << '\n';
    // std::cout << '\n';

    if (l1.x > r2.x || l2.x > r1.x)
         return false;

    // If one rectangle is above the other
    if (r1.y > l2.y || r2.y > l1.y)
        return false;

    return true;
}
