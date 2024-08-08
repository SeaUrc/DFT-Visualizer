#pragma once

#include <SFML/Graphics.hpp>

class sfLine : public sf::Drawable {
public:
    sfLine() {}

    sfLine(const sf::Vector2f &point1, const sf::Vector2f &point2) :
            color(sf::Color::Blue), thickness(5) {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (auto &vertice: vertices)
            vertice.color = color;
    }

    sfLine(const sf::Vector2f &point1, const sf::Vector2f &point2, const sf::Color col, float thick) :
            color(col), thickness(thick) {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (auto &vertice: vertices)
            vertice.color = color;
    }

    void setThickness(float thick) {
        thickness = thick;
    }

    void setColor(sf::Color c) {
        color = c;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates r) const {
        target.draw(vertices, 4, sf::Quads, r);
    }

private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};