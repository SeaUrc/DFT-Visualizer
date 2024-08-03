#include <SFML/Graphics.hpp>
#include <RoundedRectangleShape.hpp>

class Menu{
private:
    bool open = false;
    sf::RectangleShape overlay;
    RoundedRectangleShape menu;
    std::vector<sf::Text> texts;

public:
    Menu(){}

    Menu(sf::Vector2f windowSize, sf::Vector2f menuSize, float radius = 0, unsigned int cornerPoints = 30){
        overlay.setSize(windowSize);
        overlay.setFillColor(sf::Color(0, 0, 0, 128));

        menu.setSize(menuSize);
        menu.setFillColor(sf::Color(172, 172, 172));
        menu.setRadius(radius);
        menu.setCornerPointCount(cornerPoints);
        menu.setPosition((windowSize.x-menuSize.x)/2, (windowSize.y-menuSize.y)/2);
    }

    void addText(sf::Text t){
        texts.push_back(t);
    }

    void setOverlay(sf::Vector2f windowSize){
        overlay.setSize(windowSize);
        overlay.setFillColor(sf::Color(0, 0, 0, 170));
    }

    void setMenu(sf::Vector2f windowSize, sf::Vector2f menuSize, float radius = 0, unsigned int cornerPoints = 30){
        menu.setSize(menuSize);
        menu.setFillColor(sf::Color(172, 172, 172));
        menu.setRadius(radius);
        menu.setCornerPointCount(cornerPoints);
        menu.setPosition((windowSize.x-menuSize.x)/2, (windowSize.y-menuSize.y)/2);
    }

    bool isOpen() const{
        return open;
    }

    void setOpen(bool isOpen){
        open = isOpen;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates r) const
    {
        target.draw(overlay,r);
        target.draw(menu, r);

        for (size_t i = 0; i<texts.size(); ++i){
            target.draw(texts[i], r);
        }
    }

};