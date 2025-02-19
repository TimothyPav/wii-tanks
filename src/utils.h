#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>

enum class Direction { Up, Down, Left, Right, NODIRECTION };

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::RectangleShape s2, int speed);
bool doOverlap(const sf::RectangleShape& s1, const sf::RectangleShape& s2); 
bool doOverlap(const sf::CircleShape& s1, const sf::RectangleShape& s2); 
bool contains(const sf::Vector2f point, const sf::RectangleShape rectangle); 

#endif
