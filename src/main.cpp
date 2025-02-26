#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Window/Mouse.hpp>
#include <ctime>

#include "tank.h"
#include "utils.h"
#include "wall.h"


// TODO: Obstacles/walls object to put on the map!
// All the blocks will be 24x24

std::vector<Wall> level_1();

std::vector<std::unique_ptr<Tank>> tanks{};

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
    std::vector<Wall> currentLevel = level_1();
    std::vector<std::shared_ptr<Bomb>> bombs{};
    // Tank t (square, 5, currentLevel);
    // tanks.push_back(std::make_unique<Tank>(t));
    auto t_ptr = std::make_unique<Tank>(square, 2, currentLevel);
    Tank& t = *t_ptr;  // Store reference
    tanks.push_back(std::move(t_ptr));  // Move ownership
                                        //
    sf::RectangleShape enemySquare;
    auto enemy_ptr = std::make_unique<Tank>(currentLevel, 0, sf::Vector2f{1200, 300});
    Tank& enemy = *enemy_ptr;
    tanks.push_back(std::move(enemy_ptr));

    auto enemy_ptr2 = std::make_unique<Tank>(currentLevel, 0, sf::Vector2f{1200, 600});
    tanks.push_back(std::move(enemy_ptr2));

    auto enemy_ptr3 = std::make_unique<Tank>(currentLevel, 1, sf::Vector2f{600, 600});
    enemy_ptr3->setLevelTwoTank();
    tanks.push_back(std::move(enemy_ptr3));

    auto enemy_ptr4 = std::make_unique<Tank>(currentLevel, 1, sf::Vector2f{700, 400});
    enemy_ptr4->setLevelThreeTank();
    tanks.push_back(std::move(enemy_ptr4));

    bool isMousePressed { false };
    bool isSpacePressed { false };
    std::cout << "size of tanks vector: " << tanks.size() << '\n';
    double elapsed_seconds{};
    auto start_time = std::chrono::high_resolution_clock::now();

    long tempSeconds{0};
    while (window.isOpen())
    {
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
            // LOSE SCREEN
            //
            window.clear();
            window.display();
            continue;
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
                if (t.getMaxBullets() > t.getBulletSet().size()) {
                    t.shoot();
                }
            }
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

        for (int i{0}; i < currentLevel.size(); ++i)
            window.draw(currentLevel[i].getWall());

        if (t.getBomb() != nullptr) {
            window.draw(t.getBomb()->placeBomb());
        }
        
        t.rotateTurretBasedOnMouse(sf::Mouse::getPosition(window));

        for (auto& currentTank : tanks) 
        {
            if (!currentTank->getIsAlive()) continue; // skip drawing tank that is dead
            window.draw(currentTank->getTankBody());
            window.draw(currentTank->getHeadBody());
            window.draw(currentTank->getTurretBody());

            if (currentTank.get() != &t)
            {
                currentTank->rotateTurretAtPlayer(t);
            }

            if (currentTank->getIsLevelTwoTank())
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
            
            if (currentTank->getIsLevelThreeTank())
            {
                currentTank->moveTowardsPlayer(t);
            }

            // clean up bullets vector
            for (auto bullet = currentTank->getBulletSet().begin(); bullet != currentTank->getBulletSet().end(); ) {
                if (bullet->getBounces() <= 0) {
                    bullet = currentTank->getBulletSet().erase(bullet);
                } else {
                    bullet->move(window, currentLevel, bombs, tanks);
                    window.draw(bullet->getBody());
                    ++bullet;
                }
            }
        }

        window.display();
    }
}
