#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>

enum class Direction { Up, Down, Left, Right };

bool doOverlap(sf::RectangleShape s1, sf::RectangleShape s2);

#endif
