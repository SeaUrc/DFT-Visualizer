#include <SFML/Graphics.hpp>
#include <Point.hpp>
#include <Lines.hpp>

struct MenuButton{
    sf::Vector2f position;
    sf::Vector2f shape;
    sf::Color color;
    bool isOpen;
    float lineThickness = 2;

    MenuButton(){}

    MenuButton(sf::Vector2f pos, sf::Vector2f s, sf::Color c): position(pos), shape(s), color(c){
        isOpen = false;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates r) const{
        sf::Vector2f p1 = sf::Vector2f(position.x, position.y + lineThickness/2);
        sf::Vector2f p2 = sf::Vector2f(position.x+shape.x, position.y + lineThickness/2);
        sf::Vector2f p3 = sf::Vector2f(position.x, position.y + shape.y/2);
        sf::Vector2f p4 = sf::Vector2f(position.x+shape.x, position.y + shape.y/2);
        sf::Vector2f p5 = sf::Vector2f(position.x, position.y + + shape.y - lineThickness/2);
        sf::Vector2f p6 = sf::Vector2f(position.x+shape.x, position.y + shape.y - lineThickness/2);

        if (isOpen){
            sfLine l1(p1, p6, color, lineThickness);
            sfLine l2(p2, p5, color, lineThickness);
            l1.draw(target, r);
            l2.draw(target, r);
        }else{
            sfLine l1(p1, p2, color, lineThickness);
            sfLine l2(p3, p4, color, lineThickness);
            sfLine l3(p5, p6, color, lineThickness);
            l1.draw(target, r);
            l2.draw(target, r);
            l3.draw(target, r);
        }
    }

    sf::Vector2f getShape(){
        return shape;
    }

    sf::Vector2f getPosition(){
        return position;
    }
};

class Menu{
private:
    MenuButton button;
//    sf::Sprite sprite;
    sf::RectangleShape background;
    std::vector<sf::Text> options;
    sf::Font font;
    bool isOpen;

public:
    Menu(sf::Vector2f size){

        if (!font.loadFromFile("../font/font.ttf"))
        {
            throw std::invalid_argument("Can't load font!");
        }

        button = MenuButton(sf::Vector2f(20, 20), sf::Vector2f(25,25), sf::Color::White);
//        sf::Texture texture;
//        if (!texture.loadFromFile("../icon/settings-64.png")){
//            throw std::invalid_argument("Can't load settings icon!");
//        }
//        sprite = sf::Sprite(texture);
//        sprite.setScale(2, 2);
//        sprite.setPosition(300, 300);

        background = sf::RectangleShape(size);
//        std::cout << Point(sf::VideoMode().getDesktopMode().width/2, sf::VideoMode().getDesktopMode().height/2) << std::endl;
        background.setPosition(sf::VideoMode().getDesktopMode().width/4, sf::VideoMode().getDesktopMode().height/4);
    }

    void addOption(){

    }

    void draw(sf::RenderTarget &target, sf::RenderStates r){
        target.draw(background, r);
//        target.draw(sprite, r);
        button.draw(target, r);
    }
};