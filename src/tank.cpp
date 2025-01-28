#include <iostream>
#include <SFML/Graphics.hpp>

#include "tank.h"

Tank::Tank(sf::RectangleShape body)
    : body{ body },
    Tank::setDefaults()
{
}

sf::RectangleShape Tank::getBody() const {
    return body;
}

void Tank::test() const {
    std::cout << "hello from tank\n";
}
