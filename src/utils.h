#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>

enum class Direction { Up, Down, Left, Right, NODIRECTION };

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::RectangleShape s2, int speed);

#endif
