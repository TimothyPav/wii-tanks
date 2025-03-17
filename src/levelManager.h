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

inline void drawWalls(int x1, int x2, int y1, int y2, bool isHole=false)
{
    for (int i{x1}; i <= x2; ++i) {
        for (int j{y1}; j <= y2; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, isHole});
        }
    }
}

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
    player.setPos({100, 100});

    // auto enemy_ptr = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{1500, 780});
    auto enemy_ptr = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{850, 650});
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

inline void level4 (Tank& player)
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

    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2, sf::Vector2f{1360, 130});
    enemy_ptr1->setLevelTwoTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2, sf::Vector2f{1360, 815});
    enemy_ptr2->setLevelTwoTank();
    auto enemy_ptr3 = std::make_unique<Tank>(currLevel, 0, sf::Vector2f{700, 815});
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
    tanks.push_back(std::move(enemy_ptr3));
}

inline void level5(Tank& player)
{
    for (int i{4}; i <= 8; ++i) {
        for (int j{9}; j <= 11; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{6}; i <= 8; ++i) {
        for (int j{12}; j <= 13; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{22}; i <= 24; ++i) {
        for (int j{3}; j <= 4; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{22}; i <= 26; ++i) {
        for (int j{5}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
// (1626, 553)
// (1701, 179)
    player.setPos({2*60, 60*15});

    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{1620, 550});
    enemy_ptr1->setLevelThreeTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{1220, 80});
    enemy_ptr2->setLevelThreeTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
}

inline void level6(Tank& player)
{
    for (int i{4}; i <= 27; ++i) {
        for (int j{4}; j <= 5; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{4}; i <= 27; ++i) {
        for (int j{12}; j <= 13; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{11}; i <= 11; ++i) {
        for (int j{6}; j <= 9; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }
    for (int i{12}; i <= 12; ++i) {
        for (int j{9}; j <= 11; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta, true});
        }
    }

    player.setPos({60*4, 60*8});
    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{60*23, 60*7});
    enemy_ptr1->setLevelTwoTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{60*29, 60*9});
    enemy_ptr2->setLevelThreeTank();
    auto enemy_ptr3 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{60*29, 60*12});
    enemy_ptr3->setLevelThreeTank();
    auto enemy_ptr4 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{60*29, 60*15});
    enemy_ptr4->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
    tanks.push_back(std::move(enemy_ptr3));
    tanks.push_back(std::move(enemy_ptr4));

}

inline void level7 (Tank& player)
{
    for (int i{0}; i <= 12; ++i) {
        for (int j{3}; j <= 4; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{0}; i <= 12; ++i) {
        for (int j{13}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{19}; i <= 31; ++i) {
        for (int j{8}; j <= 9; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    player.setPos({28*60, 13*60});

    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{3*60, 15*60});
    enemy_ptr1->setLevelFourTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{3*60, 8*60});
    enemy_ptr2->setLevelThreeTank();
    auto enemy_ptr3 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{28*60, 4*60});
    enemy_ptr3->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
    tanks.push_back(std::move(enemy_ptr3));
}

inline void level8 (Tank& player)
{
    for (int i{4}; i <= 4; ++i) {
        for (int j{3}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{5}; i <= 5; ++i) {
        for (int j{7}; j <= 10; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{4}; i <= 4; ++i) {
        for (int j{10}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{11}; i <= 21; ++i) {
        for (int j{3}; j <= 3; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{11}; i <= 21; ++i) {
        for (int j{14}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{27}; i <= 27; ++i) {
        for (int j{3}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{26}; i <= 26; ++i) {
        for (int j{7}; j <= 10; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{27}; i <= 27; ++i) {
        for (int j{10}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    player.setPos({2*60, 13*60});
    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{9*60, 5*60});
    enemy_ptr1->setLevelFourTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{21*60, 5*60});
    enemy_ptr2->setLevelFourTank();
    auto enemy_ptr3 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{21*60, 11*60});
    enemy_ptr3->setLevelFourTank();
    auto enemy_ptr4 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{29*60, 2*60});
    enemy_ptr4->setLevelThreeTank();
    auto enemy_ptr5 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{29*60, 16*60});
    enemy_ptr5->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
    tanks.push_back(std::move(enemy_ptr3));
    tanks.push_back(std::move(enemy_ptr4));
    tanks.push_back(std::move(enemy_ptr5));
}

inline void level9 (Tank& player)
{
    for (int i{0}; i <= 6; ++i) {
        for (int j{7}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{25}; i <= 31; ++i) {
        for (int j{7}; j <= 7; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{14}; i <= 15; ++i) {
        for (int j{3}; j <= 3; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{14}; i <= 14; ++i) {
        for (int j{4}; j <= 9; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{15}; i <= 15; ++i) {
        for (int j{9}; j <= 13; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }
    for (int i{14}; i <= 15; ++i) {
        for (int j{14}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    player.setPos({4*60, 13*60});
    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{3*60, 4*60});
    enemy_ptr1->setLevelFourTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{27*60, 15*60});
    enemy_ptr2->setLevelFourTank();
    auto enemy_ptr3 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{8*60, 2*60});
    enemy_ptr3->setLevelFourTank();
    auto enemy_ptr4 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{20*60, 5*60});
    enemy_ptr4->setLevelTwoTank();
    auto enemy_ptr5 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{28*60, 3*60});
    enemy_ptr5->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
    tanks.push_back(std::move(enemy_ptr3));
    tanks.push_back(std::move(enemy_ptr4));
    tanks.push_back(std::move(enemy_ptr5));
}

inline void level10 (Tank& player)
{
    for (int i{14}; i <= 15; ++i) {
        for (int j{14}; j <= 14; ++j) {
            currLevel.push_back(Wall {i, j, sf::Color::Magenta});
        }
    }

    drawWalls(3, 8, 2, 2, false);
    drawWalls(3, 8, 8, 8, false);
    drawWalls(12, 19, 4, 4, false);
    drawWalls(12, 19, 12, 12, false);
    drawWalls(23, 28, 2, 2);
    drawWalls(23, 28, 8, 8);
    drawWalls(23, 28, 14, 14);

    player.setPos({5*60, 14*60});

    auto enemy_ptr1 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{8*60, 4*60});
    enemy_ptr1->setLevelFiveTank();
    auto enemy_ptr2 = std::make_unique<Tank>(currLevel, 2.5, sf::Vector2f{20*60, 9*60});
    enemy_ptr2->setLevelFiveTank();
    tanks.push_back(std::move(enemy_ptr1));
    tanks.push_back(std::move(enemy_ptr2));
}

typedef void (*setupLevel)(Tank& player);
class LevelManager
{
    std::array<setupLevel, 10> levels{};
    Tank& m_player;

public:
    LevelManager(Tank& player)
        : m_player(player) 
    {
        // fill level manager up?
        // levels[0] = level0;
        levels[0] = level1;
        levels[1] = level2;
        levels[2] = level3;
        levels[3] = level4;
        levels[4] = level5;
        levels[5] = level6;
        levels[0] = level7;
        levels[7] = level8;
        levels[8] = level9;
        levels[9] = level10;
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
        // bullets.clear();
        for (auto bullet = bullets.begin(); bullet != bullets.end(); ) {
            --(bullet->decrementOwner()->currentBullets);
            bullet = bullets.erase(bullet);
        }
    }

    Tank& getPlayer(){ return m_player; }
    
};

#endif
