#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#include <SFML/Graphics.hpp>
#include "bullet.h"

enum class Direction { Up, Down, Left, Right, NODIRECTION };

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::RectangleShape s2, int speed);
bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::CircleShape circle, int speed);
bool doOverlap(const sf::RectangleShape& s1, const sf::RectangleShape& s2); 
bool doOverlap(const sf::CircleShape& s1, const sf::RectangleShape& s2); 
bool contains(const sf::Vector2f point, const sf::RectangleShape rectangle); 
std::pair<Direction, Direction> getDirection(int choice);

bool doOverlap2(const sf::RectangleShape& s1, const sf::RectangleShape& s2);

void printSide(WallSide side);

#endif
