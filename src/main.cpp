#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Window/Mouse.hpp>
#include <ctime>

#include "tank.h"
#include "utils.h"
#include "wall.h"
#include "levelManager.h"


// TODO: Obstacles/walls object to put on the map!
// All the blocks will be 24x24

std::vector<Wall> level_1();

std::vector<std::unique_ptr<Tank>> tanks{};
std::vector<Bullet> bullets{};
std::vector<std::shared_ptr<Bomb>> bombs{};
std::vector<Wall> currLevel;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920, 1080}), "wii tanks");
    window.setFramerateLimit(144);

    sf::RectangleShape square;
    square.setSize(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(sf::Vector2f(100, 100));

    sf::Color color = sf::Color::Green;

    // Tank t(square, 5);
    // std::vector<Wall> currentLevel = level_1();
    // Tank t (square, 5, currentLevel);
    // tanks.push_back(std::make_unique<Tank>(t));
                                        //
    // auto enemy_ptr = std::make_unique<Tank>(currentLevel, 0, sf::Vector2f{1500, 780});
    // tanks.push_back(std::move(enemy_ptr));

    // auto enemy_ptr2 = std::make_unique<Tank>(currentLevel, 0, sf::Vector2f{1200, 600});
    // tanks.push_back(std::move(enemy_ptr2));

    // auto enemy_ptr3 = std::make_unique<Tank>(currentLevel, 1, sf::Vector2f{600, 600});
    // enemy_ptr3->setLevelTwoTank();
    // tanks.push_back(std::move(enemy_ptr3));

    // auto enemy_ptr4 = std::make_unique<Tank>(currentLevel, 1, sf::Vector2f{700, 400});
    // enemy_ptr4->setLevelThreeTank();
    // tanks.push_back(std::move(enemy_ptr4));

    // auto enmey_ptr5 = std::make_unique<Tank>(currentLevel, 2.5, sf::Vector2f{300, 800});
    // enmey_ptr5->setLevelFourTank();
    // tanks.push_back(std::move(enmey_ptr5));

    // level 5 tank
    // auto enemy_ptr6{ std::make_unique<Tank>(currentLevel, 2, sf::Vector2f{ 500, 300 }) };
    // enemy_ptr6->setLevelFiveTank();
    // tanks.push_back(std::move(enemy_ptr6));

    bool isMousePressed { false };
    bool isSpacePressed { false };
    double elapsed_seconds{};
    auto start_time = std::chrono::high_resolution_clock::now();

    long tempSeconds{0};
    auto t_ptr = std::make_unique<Tank>(square, 2, currLevel);
    Tank& t = *t_ptr;  // Store reference
    t.setPlayer();

    tanks.push_back(std::move(t_ptr));  // Move ownership
    LevelManager levelManager{t};
    int level{0};
    levelManager[level](t);
    int lives{3};

    while (window.isOpen())
    {
        if (t.getIsAlive() && tanks.size() == 1)
        {
            ++level;
            levelManager[level](t);
        }
        auto current_time = std::chrono::high_resolution_clock::now();
        long seconds { std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() };
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                isMousePressed = false;
            }
            if (event->is<sf::Event::KeyReleased>())
            {
                isSpacePressed = false;
            }
        }


        if (!t.getIsAlive())
        {
            if (lives > 1)
            {
                --lives;
                t.revive();
                levelManager[level](t);
            }
            else 
            {
                window.clear();
                window.display();
                continue;
            }
            // LOSE SCREEN
            //
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            // std::cout << "Both UP and LEFT are pressed";
            t.moveTank(Direction::Up, Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Up, Direction::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Down, Direction::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            t.moveTank(Direction::Down, Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            // std::cout << "Only UP is pressed";
            t.moveTank(Direction::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            t.moveTank(Direction::Down);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            t.moveTank(Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            t.moveTank(Direction::Right);
        }

        // MOUSE CLICK DETECTED HERE
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !isMousePressed)
        {
            // std::cout << "Bullets in set: " << t.getBulletSet().size() << '\n';
            isMousePressed = true;
            if (t.getIsAlive()) {
                if (t.getMaxBullets() > t.currentBullets) {
                    t.shoot();
                }
            }
            std::cout << "(" << sf::Mouse::getPosition().x-45 << ", " << sf::Mouse::getPosition().y-45 << ")\n";
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isSpacePressed)
        {
            if (!t.getIsAlive()) continue;
            isSpacePressed = true;
            t.plantBomb();
            bombs.push_back(t.getBomb());
        }

        window.clear();
        // do things in here?

        for (int i{0}; i < currLevel.size(); ++i)
            window.draw(currLevel[i].getWall());

        if (t.getBomb() != nullptr) {
            window.draw(t.getBomb()->placeBomb());
        }
        
        t.rotateTurretBasedOnMouse(sf::Mouse::getPosition(window));

        for (auto& currentTank : tanks) 
        {
            window.draw(currentTank->getTankBody());
            window.draw(currentTank->getHeadBody());
            window.draw(currentTank->getTurretBody());

            if (currentTank.get() != &t)
            {
                currentTank->rotateTurretAtPlayer(t);
            }

            if (currentTank->getIsLevelTwoTank()) // move randomly tank
            {
                // move this tank somehow
                currentTank->moveTank(currentTank->getDir().first, currentTank->getDir().second);
                if (seconds % 2 == 0 && !currentTank->state1)
                {
                    currentTank->changeDir();
                    currentTank->state1 = true;
                }
                if (currentTank->everySecond != seconds)
                {
                    currentTank->state1 = false;
                    currentTank->everySecond = seconds;
                }
            }
            
            if (currentTank->getIsLevelThreeTank()) // follow player tank
            {
                currentTank->moveTank(currentTank->getDir().first, currentTank->getDir().second);
                if (seconds % 2 == 0 && !currentTank->state1)
                {
                    currentTank->moveTowardsPlayer(t);
                    currentTank->state1 = true;
                }
                if (currentTank->everySecond != seconds)
                {
                    currentTank->state1 = false;
                    currentTank->everySecond = seconds;
                }
            }

            if(currentTank->getIsLevelFourTank()) // plant bombs tank
            {
                currentTank->moveTank(currentTank->getDir().first, currentTank->getDir().second);
                currentTank->plantBombEnemy(4); // 4 is max bombs for level four tank (yellow)

                if (seconds % 2 == 0 && !currentTank->state1)
                {
                    if (Random::get(1, 2) == 1)
                        currentTank->moveTowardsPlayer(t);
                    else
                    {
                        currentTank->changeDir();
                        currentTank->moveTank(currentTank->getDir().first, currentTank->getDir().second);
                    }
                    currentTank->state1 = true;
                }
                if (currentTank->everySecond != seconds)
                {
                    currentTank->state1 = false;
                    currentTank->everySecond = seconds;
                }
            }

            if(currentTank->getIsLevelFiveTank())
            {
                currentTank->moveTank(currentTank->getDir().first, currentTank->getDir().second);
                currentTank->plantBombEnemy(1); // 1 is max bombs for level five tank (black)
                if (seconds % 2 == 0 && !currentTank->state1)
                {
                    currentTank->moveTowardsPlayer(t);
                    currentTank->state1 = true;
                }
                if (currentTank->everySecond != seconds)
                {
                    currentTank->state1 = false;
                    currentTank->everySecond = seconds;
                }
            }

            if (currentTank->getIsBombPlaced() && currentTank->getBomb()->getTime() > 5)
            {
                currentTank->getBomb()->explode(tanks);
            }
        }
        // clean up bullets vector
        for (auto bullet = bullets.begin(); bullet != bullets.end(); ) {
            for (auto bulletj = bullets.begin(); bulletj != bullets.end(); ) {
                if (bullet != bulletj && doOverlap(bullet->getBody(), bulletj->getBody()))
                {
                    bullet->setZeroBounces();
                    bulletj->setZeroBounces();
                }
                ++bulletj;
            }
            if (bullet->getBounces() <= 0) {
                --(bullet->decrementOwner()->currentBullets);
                bullet = bullets.erase(bullet);
            } else {
                bullet->move(window, currLevel, bombs, tanks);
                window.draw(bullet->getBody());
                ++bullet;
            }
        }

        // clean tanks vector
        for (auto tank = tanks.begin(); tank != tanks.end(); )
        {
            if ((tank->get() != &t) && !tank->get()->getIsAlive())
                tanks.erase(tank);
            else 
                ++tank;
        }

        // clean up and handle bombs vector
        for (auto bomb = bombs.begin(); bomb != bombs.end(); )
        {
            if (!bomb->get()->getIsActive())
            {
                bombs.erase(bomb);
            }
            else 
            {
                if (bomb->get()->getTime() > 5) bomb->get()->explode(tanks);
                window.draw(bomb->get()->getBombBody());
                ++bomb;
            }
        }

        window.display();
    }
}
