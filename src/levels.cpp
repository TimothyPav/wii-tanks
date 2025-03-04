#include <iostream>
#include <SFML/Graphics.hpp>

#include "wall.h"

std::vector<Wall> level_1()
{
    std::vector<Wall> level;
    for (int i{8}; i <= 12; ++i) {
        for (int j{4}; j <= 7; ++j) {
            level.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    for (int i{8}; i <= 12; ++i) {
        for (int j{11}; j <= 14; ++j) {
            level.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }


    for (int i{19}; i <= 28; ++i) {
        for (int j{4}; j <= 7; ++j) {
            level.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    return level;
}
