#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Window/Mouse.hpp>

#include "tank.h"
#include "utils.h"
#include "wall.h"


// TODO: Obstacles/walls object to put on the map!
// All the blocks will be 24x24

std::vector<Wall> level_1();

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
    std::vector<Tank*> tanks{};
    Tank t (square, 5, currentLevel);
    tanks.push_back(&t);
    bool isMousePressed { false };
    bool isSpacePressed { false };

    while (window.isOpen())
    {
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

        for (std::size_t i{0}; i < tanks.size(); ++i)
        {
            Tank currentTank{ *tanks[i] };
            if (!currentTank.getIsAlive()) continue; // skip drawing tank that is dead
            window.draw(currentTank.getTankBody());
            window.draw(currentTank.getHeadBody());
            window.draw(currentTank.getTurretBody());
        }

        // clean up bullets vector
        for (auto bullet = t.getBulletSet().begin(); bullet != t.getBulletSet().end(); ) {
            if (bullet->getBounces() <= 0) {
                bullet = t.getBulletSet().erase(bullet);
            } else {
                bullet->move(window, currentLevel, bombs, tanks);
                window.draw(bullet->getBody());
                ++bullet;
            }
        }

        //clean up tanks vector
        for (auto tank = tanks.begin(); tank != tanks.end(); ) {
            if (!(*tank)->getIsAlive())
            {
                delete *tank;
                tanks.erase(tank);
            } else {
                ++tank;
            }
        }
        std::cout << "size of tanks* vector: " << tanks.size() << '\n';

        window.display();
    }
}
