#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Window/Mouse.hpp>
#include <ctime>
#include <array>
#include <vector>

#include "tank.h"
#include "utils.h"
#include "wall.h"
#include "levelManager.h"
#include "Animation.h"


// TODO: Obstacles/walls object to put on the map!
// All the blocks will be 24x24

std::vector<std::unique_ptr<Tank>> tanks{};
std::vector<Bullet> bullets{};
std::vector<std::shared_ptr<Bomb>> bombs{};
std::vector<Wall> currLevel;

using spriteVector = std::vector<sf::Sprite>;
void displayLevel(sf::RenderWindow& window, sf::Sprite& s, sf::Sprite& wallArt, spriteVector& bodies, spriteVector& heads, spriteVector& turrets);

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920, 1080}), "wii tanks");
    window.setFramerateLimit(144);

    sf::RectangleShape square;
    square.setSize(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(sf::Vector2f(100, 100));

    sf::Color color = sf::Color::Green;

    sf::RectangleShape testBomb({89, 89});
    testBomb.setSize({100, 100});
    sf::Texture explosionTexture;
    auto p = explosionTexture.loadFromFile("../assets/explosion.png");
    testBomb.setTexture(&explosionTexture);

    Animation animation(&explosionTexture, sf::Vector2u(6, 1), 0.1f);
    float deltaTime{0};
    sf::Clock clock;

    sf::Texture startScreenTexture;
    p = startScreenTexture.loadFromFile("../assets/title_screen.png");
    sf::Sprite startScreen(startScreenTexture);

    sf::Texture pic;
    p = pic.loadFromFile("../assets/background.jpg");
    sf::Sprite s(pic);


    sf::Texture wallArtTexture;
    p = wallArtTexture.loadFromFile("../assets/block1.png");
    sf::Sprite wallArt(wallArtTexture);

    sf::Texture tankBodiesTexture;
    p = tankBodiesTexture.loadFromFile("../assets/tankSprites.png");
    std::vector<sf::Sprite> tankSprites_bodies;
    for (int i{0}; i < 6; ++i)
    {
        sf::Sprite tankSprite(tankBodiesTexture);
        tankSprite.setTextureRect(sf::IntRect({i*50, 0}, {50, 50}));
        tankSprites_bodies.push_back(tankSprite);
    }
    std::vector<sf::Sprite> tankSprites_heads;
    for (int i{0}; i < 6; ++i)
    {
        sf::Sprite tankSprite(tankBodiesTexture);
        tankSprite.setTextureRect(sf::IntRect({i*50, 50}, {30, 30}));
        tankSprites_heads.push_back(tankSprite);
    }
    std::vector<sf::Sprite> tankSprites_turrets;
    for (int i{0}; i < 6; ++i)
    {
        sf::Sprite tankSprite(tankBodiesTexture);
        tankSprite.setTextureRect(sf::IntRect({i*50, 80}, {50, 10}));
        tankSprites_turrets.push_back(tankSprite);
    }

    bool isMousePressed { false };
    bool isSpacePressed { false };
    auto start_time = std::chrono::high_resolution_clock::now();

    auto t_ptr = std::make_unique<Tank>(square, 1.5, currLevel);
    Tank& t = *t_ptr;  // Store reference
    t.setPlayer();

    tanks.push_back(std::move(t_ptr));  // Move ownership
    LevelManager levelManager{t};
    int level{0};
    levelManager[level](t);
    int lives{3};

    bool gameStart{ false };

    while (window.isOpen())
    {

        auto current_time = std::chrono::high_resolution_clock::now();
        long seconds { std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() };
        if (t.getIsAlive() && tanks.size() == 1)
        {
            ++level;
            levelManager[level](t);
            t.resetRotation();
            displayLevel(window, s, wallArt, tankSprites_bodies, tankSprites_heads, tankSprites_turrets);
        }

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

        if (!gameStart)
        {
            window.draw(startScreen);
            window.display();
            window.clear();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                gameStart = true;
                isSpacePressed = true;
            }
            continue;
        }

        if (!t.getIsAlive())
        {
            if (lives > 1)
            {
                std::cout << "dies...\n";
                --lives;
                t.revive();
                levelManager[level](t);
                displayLevel(window, s, wallArt, tankSprites_bodies, tankSprites_heads, tankSprites_turrets);
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            // std::cout << "Both UP and LEFT are pressed";
            t.moveTank(Direction::Up, Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            t.moveTank(Direction::Up, Direction::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            t.moveTank(Direction::Down, Direction::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            t.moveTank(Direction::Down, Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            // std::cout << "Only UP is pressed";
            t.moveTank(Direction::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            t.moveTank(Direction::Down);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            t.moveTank(Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
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
            // std::cout << "(" << sf::Mouse::getPosition().x-45 << ", " << sf::Mouse::getPosition().y-45 << ")\n";
        }
        if(gameStart && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isSpacePressed)
        {
            if (!t.getIsAlive()) continue;
            isSpacePressed = true;
            t.plantBomb();
            bombs.push_back(t.getBomb());
        }

        window.clear();
        // do things in here?
        window.draw(s);

        for (int i{0}; i < currLevel.size(); ++i) {
            // Get the current wall rectangle
            sf::RectangleShape wall = currLevel[i].getWall();
    
            // Set the sprite position and rotation to match the wall
            wallArt.setPosition(wall.getPosition());
            window.draw(wallArt);
        }

        if (t.getBomb() != nullptr) {
            window.draw(t.getBomb()->placeBomb());
        }
        
        t.rotateTurretBasedOnMouse(sf::Mouse::getPosition(window));

        // clean up and handle bombs vector
        deltaTime = clock.restart().asSeconds();
        for (auto bomb = bombs.begin(); bomb != bombs.end(); )
        {
            if (!bomb->get()->getIsActive() && bomb->get()->isAnimationFinished())
            {
                bombs.erase(bomb);
            }
            else 
            {
                if (bomb->get()->getTime() > 5) bomb->get()->explode(tanks);
                bomb->get()->animate(deltaTime);
                window.draw(bomb->get()->getBombBody());
                ++bomb;
            }
        }

        for (auto& currentTank : tanks) 
        {
            if (!currentTank->getIsAlive()) continue;
            window.draw(getBodySprite(tankSprites_bodies, currentTank.get()));
            window.draw(getHeadSprite(tankSprites_heads, currentTank.get()));
            window.draw(getTurretSprite(tankSprites_turrets, currentTank.get()));

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
                currentTank->plantBombEnemy(4, t); // 4 is max bombs for level four tank (yellow)

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

            if(currentTank->getIsLevelFiveTank())
            {
                currentTank->moveTank(currentTank->getDir().first, currentTank->getDir().second);
                currentTank->plantBombEnemy(1, t); // 1 is max bombs for level five tank (black)

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
            if ((tank->get() != &t) && !tank->get()->getIsAlive() && tank->get()->isAnimationFinished())
                tanks.erase(tank);
            else if ((tank->get() != &t) && !tank->get()->getIsAlive() && !tank->get()->isAnimationFinished())
            {
                tank->get()->animate(deltaTime);
                window.draw(tank->get()->getTankBody());
                ++tank;
            }
            else {
                ++tank;
            }
        }
        window.draw(t.getTankBody());

        window.display();
    }
}

void displayLevel(sf::RenderWindow& window, sf::Sprite& s, sf::Sprite& wallArt, spriteVector& bodies, spriteVector& heads, spriteVector& turrets)
{
    std::cout << "displayLevel...\n";
    window.clear();
    window.draw(s);
    for (int i{0}; i < currLevel.size(); ++i) {
        // Get the current wall rectangle
        sf::RectangleShape wall = currLevel[i].getWall();
    
        // Set the sprite position and rotation to match the wall
        wallArt.setPosition(wall.getPosition());
        window.draw(wallArt);
    }

    for (auto& currentTank : tanks) 
    {
        window.draw(getBodySprite(bodies, currentTank.get()));
        window.draw(getHeadSprite(heads, currentTank.get()));
        window.draw(getTurretSprite(turrets, currentTank.get()));
    }

    window.display();
    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    long waitTime { std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() };
    while (waitTime < 3)
    {
        auto end = std::chrono::high_resolution_clock::now();
        waitTime = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }
}
