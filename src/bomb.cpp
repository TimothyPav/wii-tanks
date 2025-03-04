#include <SFML/Graphics.hpp>

#include "bomb.h"
#include "tank.h"
#include "utils.h"

// m_body will belong to one of the Tank class objects from std::vector<Tank*>&
void Bomb::explode(const std::vector<std::unique_ptr<Tank>>& tanks) {
    if (!isActive) return;
    m_body.setOrigin({m_explosionRadius/1.3f, m_explosionRadius/1.3f});
    m_body.setRadius(m_explosionRadius);


    for (auto& currentTank : tanks) {
        if (currentTank == nullptr) continue;
        if (doOverlap(m_body, currentTank->getTankBody()))
        {
            // std::cout << "tank should be killed right now!\n";
            currentTank->kill();
        }

        if (&currentTank->getBomb()->m_body == &m_body)
        {
            // std::cout << "bomb should reset right now!\n";
            currentTank->resetBomb();
        }
        if (currentTank->getIsLevelFourTank() && currentTank->m_bombVector.size() > 0)
        {
            for (auto bomb = currentTank->m_bombVector.begin(); bomb != currentTank->m_bombVector.end(); )
            {
                if (bomb->get() == this) {
                    bomb = currentTank->m_bombVector.erase(bomb);
                    // std::cout << "1 bomb was just deleted from level 4 tank\n";
                    break;
                }
                else {
                    ++bomb;
                }
            }
        }
    }
    isActive = false;
}

