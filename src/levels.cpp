#include <iostream>
#include <SFML/Graphics.hpp>

#include "wall.h"

std::vector<Wall> level_1()
{
    std::vector<Wall> level;
    Wall w1{ 0, 0, sf::Color::Magenta }; 
    level.push_back(w1);
    // Wall w2{ 1, 0, sf::Color::Magenta }; 
    // level.push_back(w2);
    // Wall w3{ 2, 0, sf::Color::Magenta }; 
    // level.push_back(w3);

    return level;
}
