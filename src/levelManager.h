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

inline void level2(Tank& player)
{
    for (int i{5}; i <= 27; ++i) {
        for (int j{3}; j <= 6; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    for (int i{5}; i <= 27; ++i) {
        for (int j{10}; j <= 13; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    player.setPos({60, 480+25});

    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{1740, 530});
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2, sf::Vector2f{1740, 940});
    enemy_ptr2->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
}

inline void level3(Tank& player)
{
    for (int i{5}; i <= 17; ++i) {
        for (int j{3}; j <= 4; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{18}; i <= 19; ++i) {
        for (int j{3}; j <= 9; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{16}; i <= 17; ++i) {
        for (int j{8}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{18}; i <= 27; ++i) {
        for (int j{13}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    player.setPos({300, 480});
    // 120, 165
    // 1611, 1041 

    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{1500, 200});
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2, sf::Vector2f{120, 125});
    enemy_ptr2->setLevelTwoTank();
    auto enemy_ptr3 = std::make_unique<Tank>(currLevel, 2, sf::Vector2f{1600, 950});
    enemy_ptr3->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
    tanks.push_back(std::move(enemy_ptr3));
}

inline void level0 (Tank& player)
{
    for (int i{0}; i <= 5; ++i) {
        for (int j{4}; j <= 4; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    for (int i{15}; i <= 31; ++i) {
        for (int j{4}; j <= 4; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    for (int i{0}; i <= 15; ++i) {
        for (int j{12}; j <= 12; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    for (int i{25}; i <= 31; ++i) {
        for (int j{12}; j <= 12; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    for (int i{10}; i <= 10; ++i) {
        for (int j{0}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    for (int i{20}; i <= 20; ++i) {
        for (int j{10}; j <= 17; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    player.setPos({300, 480});
    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{1500, 200});
    tanks.push_back(std::move(enemy_ptr1));
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
        levels[0] = level0;
        // levels[0] = level1;
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
            };
        }

        bombs.clear();
        currLevel.clear();
        bullets.clear();
    }

    Tank& getPlayer(){ return m_player; }
    
};

#endif
