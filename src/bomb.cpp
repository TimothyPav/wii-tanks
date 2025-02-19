#include <SFML/Graphics.hpp>

#include "bomb.h"
#include "tank.h"
#include "utils.h"

bool Bomb::explode(std::vector<Tank>& tanks) {
    std::cout << "hello from explode\n";
    m_body.setOrigin({m_explosionRadius/1.3f, m_explosionRadius/1.3f});
    m_body.setRadius(m_explosionRadius);

    for (int i{0}; i < tanks.size(); ++i)
    {
        // FIX THIS NOW!!
    }

    return false;
}

