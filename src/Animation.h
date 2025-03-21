#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation 
{
public:
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);

    void loopUpdate(int row, float deltaTime);
    bool playAnimation(int row, float deltaTime);

public:
    sf::IntRect m_uvRect;

private:
    sf::Vector2u m_imageCount;
    sf::Vector2u m_currentImage;

    float m_totalTime;
    float m_switchTime;
};

#endif
