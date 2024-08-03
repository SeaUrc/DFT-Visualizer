#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape
{
public:
    RoundedRectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0), float radius = 0, unsigned int cornerPoints = 30)
    {
        this->size = size;
        this->radius = radius;
        this->cornerPoints = cornerPoints;
        update();
    }

    void setSize(const sf::Vector2f& size)
    {
        this->size = size;
        update();
    }

    const sf::Vector2f& getSize() const
    {
        return size;
    }

    void setRadius(float radius)
    {
        this->radius = radius;
        update();
    }

    float getRadius() const
    {
        return radius;
    }

    void setCornerPointCount(unsigned int count)
    {
        this->cornerPoints = count;
        update();
    }

    virtual std::size_t getPointCount() const
    {
        return cornerPoints * 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const
    {
        if (index >= getPointCount())
            return sf::Vector2f(0, 0);

        float deltaAngle = 90.0f / (cornerPoints - 1);
        float centerX = size.x - radius;
        float centerY = size.y - radius;

        unsigned int corner = index / cornerPoints;
        unsigned int point = index % cornerPoints;

        float angle = deltaAngle * point;

        switch(corner)
        {
            case 0: return sf::Vector2f(radius - std::cos(angle * M_PI / 180) * radius, radius - std::sin(angle * M_PI / 180) * radius);
            case 1: return sf::Vector2f(centerX + std::sin(angle * M_PI / 180) * radius, radius - std::cos(angle * M_PI / 180) * radius);
            case 2: return sf::Vector2f(centerX + std::cos(angle * M_PI / 180) * radius, centerY + std::sin(angle * M_PI / 180) * radius);
            case 3: return sf::Vector2f(radius - std::sin(angle * M_PI / 180) * radius, centerY + std::cos(angle * M_PI / 180) * radius);
        }

        return sf::Vector2f(0, 0);
    }

private:
    sf::Vector2f size;
    float radius;
    unsigned int cornerPoints;
};