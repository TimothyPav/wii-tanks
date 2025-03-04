#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Timer.h"

class Tank;
class Bomb
{
public:
    Bomb (float x, float y)
    {
       m_seconds = 5;
       m_body.setPosition({x, y});
       m_body.setRadius(m_radius);
       m_body.setFillColor(sf::Color::Yellow);
    }

    ~Bomb()
    {
    }

    void explode(const std::vector<std::unique_ptr<Tank>>& tanks);
    sf::CircleShape& placeBomb() { return m_body; }
    sf::CircleShape& getBombBody() { return m_body; }
    void test() const { std::cout << "hello from test in Bomb\n"; }
    double getTime() const { return time.elapsed(); }
    bool getIsActive() const { return isActive; }
    void killBomb() { isActive = false; }


private:
    int m_seconds{};
    const float m_radius{ 10.0 };
    sf::CircleShape m_body{};
    const float m_explosionRadius{ 100.0 };
    Timer time{};
    bool isActive{ true };

};

#endif
