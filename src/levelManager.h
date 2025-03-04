#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <iostream>
#include <vector>
#include <memory>

#include "tank.h"
#include "bullet.h"
#include "bomb.h"
#include "wall.h"

extern std::vector<std::unique_ptr<Tank>> tanks;
extern std::vector<Bullet> bullets;
extern std::vector<std::shared_ptr<Bomb>> bombs;
extern std::vector<Wall> currLevel;

typedef void (*setupLevel)();

inline void level1()
{
    // call some resets right here

    for (int i{8}; i <= 12; ++i) {
        for (int j{4}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{8}; i <= 12; ++i) {
        for (int j{11}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{19}; i <= 28; ++i) {
        for (int j{4}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    auto enemy_ptr = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{1500, 780});
    tanks.push_back(std::move(enemy_ptr));
    

}

class LevelManager
{
    std::vector<setupLevel> levels;


public:
    LevelManager()
    {
    }
    
};

#endif
