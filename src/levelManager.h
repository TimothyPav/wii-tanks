#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

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

inline void level1(Tank& player)
{
    // call some resets right here

    // for (int i{8}; i <= 12; ++i) {
        // for (int j{4}; j <= 7; ++j) {
            // currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        // }
    // }
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

inline void level2(Tank& player)
{
    std::cout << "level two is now being loaded\n";
    // call some resets right here
    for (int i{5}; i <= 27; ++i) {
        for (int j{4}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    for (int i{5}; i <= 27; ++i) {
        for (int j{11}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    // for (auto& shape : player.getBody())
    // {
        // shape.setPosition({2*60, 8*60});
    // }
    //


    // gotta change player POS
    std::cout << "player pos before: (" << player.getTankBody().getPosition().x << ", " << player.getTankBody().getPosition().y << ")\n";
    player.getTankBody().setPosition({0, 0});
    std::cout << "player pos after: (" << player.getTankBody().getPosition().x << ", " << player.getTankBody().getPosition().y << ")\n";

    auto enemy_ptr = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{1500, 780});
    tanks.push_back(std::move(enemy_ptr));
}

typedef void (*setupLevel)(Tank& player);
class LevelManager
{
    std::array<setupLevel, 2> levels{};
    Tank& m_player;

public:
    LevelManager(Tank& player)
        : m_player(player) 
    {
        // fill level manager up?
        levels[0] = level1;
        levels[1] = level2;
    }

    setupLevel operator[] (int level) const
    {
        resetGlobals();
        return levels[level];
    }

    void resetGlobals() const
    {
        for (auto tank = tanks.begin(); tank != tanks.end(); )
        {
            if (tank->get() != &m_player)
                tank = tanks.erase(tank);
            else
            {
                ++tank;
                std::cout << "found player tank!\n";
            };
        }

        bombs.clear();
        currLevel.clear();
        bullets.clear();
    }

    Tank& getPlayer(){ return m_player; }
    
};

#endif
