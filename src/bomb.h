#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include <iostream>



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

    bool explode(std::vector<Tank*>& tanks);
    sf::CircleShape& placeBomb() { return m_body; }
    sf::CircleShape& getBombBody() { return m_body; }
    void test() const { std::cout << "hello from test in Bomb\n"; }


private:
    int m_seconds{};
    const float m_radius{ 10.0 };
    sf::CircleShape m_body{};
    const float m_explosionRadius{ 45.0 };

};

#endif
