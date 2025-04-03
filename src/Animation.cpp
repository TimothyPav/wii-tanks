#include "Animation.h"

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount,
                     float switchTime)
    : m_imageCount(imageCount), m_switchTime(switchTime), m_totalTime(0.0f) {
    m_currentImage.x = 0;
    m_uvRect.size.x = texture->getSize().x / static_cast<float>(imageCount.x);
    m_uvRect.size.y = texture->getSize().y / static_cast<float>(imageCount.y);
}

void Animation::loopUpdate(int row, float deltaTime) {
    m_currentImage.y = row;
    m_totalTime += deltaTime;

    if (m_totalTime >= m_switchTime) {
        m_totalTime -= m_switchTime;
        ++m_currentImage.x;

        if (m_currentImage.x >= m_imageCount.x)
            m_currentImage.x = 0;
    }

    m_uvRect.position.x = m_currentImage.x * m_uvRect.size.x;
    m_uvRect.position.y = m_currentImage.y * m_uvRect.size.y;
}

bool Animation::playAnimation(int row, float deltaTime) {
    m_currentImage.y = row;
    m_totalTime += deltaTime;

    if (m_totalTime >= m_switchTime) {
        m_totalTime -= m_switchTime;
        ++m_currentImage.x;

        if (m_currentImage.x >= m_imageCount.x)
            return false;
    }

    m_uvRect.position.x = m_currentImage.x * m_uvRect.size.x;
    m_uvRect.position.y = m_currentImage.y * m_uvRect.size.y;
    return true;
}
