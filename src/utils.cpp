#include <iostream>
#include <utility>
#include <cmath>

#include "utils.h"
#include "bullet.h"

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::RectangleShape s2, int speed)
{
    sf::Vector2f pos2 = s2.getPosition();
    
    // Get size of each rectangle 
    sf::Vector2f size2 = (s2.getSize());
    
    // Calculate bottom-right corners
    sf::Vector2f bottomRight2(pos2.x + size2.x, pos2.y + size2.y);

    // std::cout << "Tank: " << tankTopLeft.x << "," << tankTopLeft.y << " to " << tankBottomRight.x << "," << tankBottomRight.y << "\n";
    // std::cout << "Rectangle 2: " << pos2.x << "," << pos2.y << " to " << bottomRight2.x << "," << bottomRight2.y << "\n";
    // std::cout << '\n';
    // 

    if (tankTopLeft.x > bottomRight2.x-speed || pos2.x+speed > tankBottomRight.x)
        return false;
    if (tankTopLeft.y > bottomRight2.y-speed || pos2.y+speed > tankBottomRight.y)
        return false;
    return true;
}

bool doOverlap(sf::Vector2f tankTopLeft, sf::Vector2f tankBottomRight, sf::CircleShape circle, int speed)
{
    // Get circle position (which is the center in SFML CircleShape)
    sf::Vector2f circleCenter = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
    
    // Get circle radius
    float radius = circle.getRadius();
    
    // Find the closest point on the rectangle to the circle
    float closestX = std::max(tankTopLeft.x, std::min(circleCenter.x, tankBottomRight.x));
    float closestY = std::max(tankTopLeft.y, std::min(circleCenter.y, tankBottomRight.y));
    
    // Calculate the distance between the closest point and the circle center
    float distanceX = circleCenter.x - closestX;
    float distanceY = circleCenter.y - closestY;
    
    // If the distance is less than the radius, the circle and rectangle overlap
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    
    // Account for the speed parameter by effectively increasing the circle's radius
    // This creates a buffer zone around the circle
    return distanceSquared <= radius  * radius;
}

bool doOverlap(const sf::RectangleShape& s1, const sf::RectangleShape& s2) {
    sf::FloatRect rect1 = s1.getGlobalBounds();
    sf::FloatRect rect2 = s2.getGlobalBounds();
    
    return rect1.findIntersection(rect2) != std::nullopt;
}

bool doOverlap2(const sf::RectangleShape& s1, const sf::RectangleShape& s2) {
    // Use the shape transforms directly - this accounts for ALL transforms including
    // origin, position, scale, and rotation - even if they were applied in multiple steps
    const sf::Transform& t1 = s1.getTransform();
    const sf::Transform& t2 = s2.getTransform();
    
    // Get the sizes to determine the local space corners
    sf::Vector2f s1Size = s1.getSize();
    sf::Vector2f s2Size = s2.getSize();
    
    // Define the local space corners (these are the same for any rectangle)
    // The order is: top-left, top-right, bottom-right, bottom-left
    sf::Vector2f localCorners[4] = {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };
    
    // Scale local corners by size to get actual corners
    // Then transform to global space
    sf::Vector2f r1Points[4];
    sf::Vector2f r2Points[4];
    
    for (int i = 0; i < 4; i++) {
        // Get rectangle 1 points in global space
        sf::Vector2f localPoint1 = {
            localCorners[i].x * s1Size.x,
            localCorners[i].y * s1Size.y
        };
        r1Points[i] = t1.transformPoint(localPoint1);
        
        // Get rectangle 2 points in global space
        sf::Vector2f localPoint2 = {
            localCorners[i].x * s2Size.x,
            localCorners[i].y * s2Size.y
        };
        r2Points[i] = t2.transformPoint(localPoint2);
    }
    
    // Calculate edges for both shapes
    sf::Vector2f edges[4] = {
        {r1Points[1].x - r1Points[0].x, r1Points[1].y - r1Points[0].y}, // Top edge of r1
        {r1Points[3].x - r1Points[0].x, r1Points[3].y - r1Points[0].y}, // Left edge of r1
        {r2Points[1].x - r2Points[0].x, r2Points[1].y - r2Points[0].y}, // Top edge of r2
        {r2Points[3].x - r2Points[0].x, r2Points[3].y - r2Points[0].y}  // Left edge of r2
    };
    
    // Calculate perpendicular vectors (normals) to use as axes
    sf::Vector2f axes[4] = {
        {-edges[0].y, edges[0].x},  // Perpendicular to top edge of r1
        {-edges[1].y, edges[1].x},  // Perpendicular to left edge of r1
        {-edges[2].y, edges[2].x},  // Perpendicular to top edge of r2
        {-edges[3].y, edges[3].x}   // Perpendicular to left edge of r2
    };
    
    // Normalize the axes
    for (int i = 0; i < 4; i++) {
        float length = std::sqrt(axes[i].x * axes[i].x + axes[i].y * axes[i].y);
        if (length > 0) {
            axes[i] = {axes[i].x / length, axes[i].y / length};
        }
    }
    
    // Check for separation along each axis
    for (int a = 0; a < 4; a++) {
        const sf::Vector2f& axis = axes[a];
        
        // Project shape 1 onto the axis
        float min1 = r1Points[0].x * axis.x + r1Points[0].y * axis.y;
        float max1 = min1;
        
        for (int i = 1; i < 4; i++) {
            float projection = r1Points[i].x * axis.x + r1Points[i].y * axis.y;
            min1 = std::min(min1, projection);
            max1 = std::max(max1, projection);
        }
        
        // Project shape 2 onto the axis
        float min2 = r2Points[0].x * axis.x + r2Points[0].y * axis.y;
        float max2 = min2;
        
        for (int i = 1; i < 4; i++) {
            float projection = r2Points[i].x * axis.x + r2Points[i].y * axis.y;
            min2 = std::min(min2, projection);
            max2 = std::max(max2, projection);
        }
        
        // Check for separation (gap between projections)
        // Add a small epsilon to account for floating point errors
        const float EPSILON = 0.001f;
        if (max1 + EPSILON < min2 || max2 + EPSILON < min1) {
            // Found a separating axis - no collision
            return false;
        }
    }
    
    // No separating axis found - collision detected
    return true;
}

bool doOverlap(const sf::CircleShape& s1, const sf::RectangleShape& s2) {
    sf::FloatRect circle = s1.getGlobalBounds();
    sf::FloatRect rect = s2.getGlobalBounds();
    
    return circle.findIntersection(rect) != std::nullopt;
}

bool contains(const sf::Vector2f point, const sf::RectangleShape rectangle) {
    sf::Vector2f topLeft({rectangle.getPosition().x, rectangle.getPosition().y});
    sf::Vector2f bottomRight({topLeft.x + rectangle.getSize().x, topLeft.y + rectangle.getSize().y});

    if (point.x > topLeft.x && point.x < bottomRight.x && point.y > topLeft.y && point.y < bottomRight.y)
        return true;
    return false;
}

std::pair<Direction, Direction> getDirection(int choice)
{
    switch(choice)
    {
    // Uses Class Template Argument Deduction
    case 0: return std::pair { Direction::Up, Direction::Left };
    case 1: return std::pair { Direction::Up, Direction::Right };
    case 2: return std::pair { Direction::Down, Direction::Right };
    case 3: return std::pair { Direction::Down, Direction::Left };
    case 4: return std::pair { Direction::Up, Direction::NODIRECTION };
    case 5: return std::pair { Direction::Down, Direction::NODIRECTION };
    case 6: return std::pair { Direction::Left, Direction::NODIRECTION };
    case 7: return std::pair { Direction::Right, Direction::NODIRECTION };
    default: return std::pair { Direction::Up, Direction::Left };
    }
}

void printSide(WallSide side)
{
    switch(side)
    {
    case WallSide::left: std::cout << "left\n"; break;
    case WallSide::right: std::cout << "right\n"; break;
    case WallSide::top: std::cout << "top\n"; break;
    case WallSide::bottom: std::cout << "bottom\n"; break;
    case WallSide::NODIRECTION: std::cout << "NODIRESCTION\n"; break;
    default: std::cout << "???\n"; break;
    }
}
