#include <SFML/Graphics.hpp>

#include "bomb.h"
#include "tank.h"
#include "utils.h"
#include "Animation.h"

// m_body will belong to one of the Tank class objects from std::vector<Tank*>&
void Bomb::explode(const std::vector<std::unique_ptr<Tank>>& tanks) {
    if (!isActive) return;

    sf::Vector2f centerPos = m_body.getPosition() + sf::Vector2f(m_body.getRadius(), m_body.getRadius());

    m_body.setRadius(m_explosionRadius);
    m_body.setOrigin({m_explosionRadius, m_explosionRadius});
    m_body.setPosition(centerPos);

    m_body.setTexture(&explosionTexture);
    animation = Animation(&explosionTexture, sf::Vector2u(11, 1), .05);
    

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
    animate(0);
}

void Bomb::animate(float deltaTime)
{
    if (isActive) return;

    if (!animation.playAnimation(0, deltaTime))
        animationFinished = true;
    m_body.setTextureRect(animation.m_uvRect);

}
