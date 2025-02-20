#include <SFML/Graphics.hpp>

#include "bomb.h"
#include "tank.h"
#include "utils.h"

// m_body will belong to one of the Tank class objects from std::vector<Tank*>&
bool Bomb::explode(std::vector<Tank*>& tanks) {
    m_body.setOrigin({m_explosionRadius/1.3f, m_explosionRadius/1.3f});
    m_body.setRadius(m_explosionRadius);

    // pointer problems!
    for (std::size_t i{0}; i < tanks.size(); ++i)
    {
        Tank* currentTank { tanks[i] };
        if (currentTank == nullptr) continue;
        if (doOverlap(m_body, currentTank->getTankBody()))
        {
            std::cout << "tank should be killed right now!\n";
            currentTank->kill();
        }

        if (&currentTank->getBomb()->m_body == &m_body)
        {
            std::cout << "bomb should reset right now!\n";
            tanks[i]->resetBomb();
        }
    }

    return false;
}

