#include <iostream>
#include <SFML/Graphics.hpp>
#include <Drawing.hpp>
#include <consts.hpp>
#include <DFT.hpp>
#include <Signal.hpp>
#include <Lines.hpp>
#include <deque>

std::vector<Cycloid> cycles;
Signal sig = Signal();
bool drawing = false;

int maxCoef = 10;
//int fadingLength = 300; // number of points that fade
//float stoppingPercentage = 0.95; // percentage of one rotation untill path disapperas
float lineThickness = 3;

void compute(Point origin){
    std::vector<complex> points = sig.getComplex();
    reverse(points.begin(), points.end());
    Fourier f = Fourier(points, origin);
    f.DFT();
    f.sortRes();
    cycles = f.constructEpicycles();
    if (cycles.size() > 0){
        drawing = true;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "DFT vectors");
    window.setVerticalSyncEnabled(true); // syncs application refresh rate to vertical freq. of monitor
    sf::Font font;
    if (!font.loadFromFile("../font/font.ttf"))
    {
        throw "Can't load font!";
    }


    std::deque<sf::Vector2f> m_path; // point, time
    Point origin(window.getSize().x/2, window.getSize().y/2);

    sf::Clock clock;
    bool clicking = false;
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::LostFocus){

            }
            if (event.type == sf::Event::GainedFocus){

            }
            if (event.type == sf::Event::MouseButtonPressed){
               clicking = true;
            }
            if (event.type == sf::Event::MouseButtonReleased && clicking){
                clicking = false;
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::C){
                    compute(origin);
                    m_path.clear();
//                    std::cout << "computed " << cycles[0] << std::endl;
                }
                if (event.key.code == sf::Keyboard::S){
                    maxCoef--;
                }
                if (event.key.code == sf::Keyboard::D){
                    maxCoef++;
                }
                if (event.key.code == sf::Keyboard::Z){
                    speedMulti -= 0.05;
                    m_path.clear(); // prevent jumping
                }
                if (event.key.code == sf::Keyboard::X){
                    speedMulti += 0.05;
                    m_path.clear(); // prevent jumping
                }
                if (event.key.code == sf::Keyboard::Q){
                    lineThickness -= 0.5;
                }
                if (event.key.code == sf::Keyboard::W){
                    lineThickness += 0.5;
                }

            }
            // sf::Event::MouseMoved
        }


        if (clicking){
            sig.addPoint(Point(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) - origin);
//            std::cout << "add point " << Point(sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y) - origin << std::endl;
        }

        window.clear(sf::Color::Black);

        /*   Text  */
        std::vector<sf::Text> texts;
        sf::Text computeFourier("Compute epicycles (C)", font, 15);
        sf::Text epicycleNumText("Number of epicycles (S: -1, D: +1): " + std::to_string(maxCoef), font, 15);
        sf::Text speedMultiText("Speed multiplier (Z: -0.05, X: +0.05): " + std::to_string(speedMulti).substr(0, 5), font, 15); // diplay up to third decimal
        sf::Text traceThickness("Trace Thickness (Q: -0.5, W: +0.5): " + std::to_string(lineThickness).substr(0, 4), font, 15);

        texts.push_back(computeFourier);
        texts.push_back(epicycleNumText);
        texts.push_back(speedMultiText);
        texts.push_back(traceThickness);

        for (int i=0; i<texts.size(); i++){
            texts[i].setFillColor(sf::Color::White);
            texts[i].setPosition(10, 10 + 25*i);
            window.draw(texts[i]);
        }



        /*   Drawing Epicycles   */
        if (drawing){
            Point pos = cycles[0].getPos();

            for (size_t i=0; i<cycles.size() && i < maxCoef; i++){
                cycles[i].update(clock.getElapsedTime(), pos);
                pos = cycles[i].getEndPoint();
            }

            m_path.push_back(sf::Vector2f(pos));

            for (size_t i=1; i<m_path.size(); ++i){
                sfLine l(m_path[i-1].first, m_path[i].first, sf::Color::Blue, lineThickness);
                l.draw(window, sf::RenderStates());
            }

            for (size_t i=0; i<cycles.size() && i < maxCoef; ++i){
                cycles[i].draw(window);
            }
        }

        window.display();

    }

    return 0;
}
