#include <SFML/Graphics.hpp>
#include <Drawing.hpp>
#include <Consts.hpp>
#include <DFT.hpp>
#include <Signal.hpp>
#include <Lines.hpp>
#include <ViewportHandler.hpp>
#include <Menu.hpp>

#include <iostream>
#include <deque>

int maxCoef = 10;
int maxPoints = 110;
float lineThickness = 1.5;

bool clicking = false;
bool tracking = false;
bool drawing = false;

std::vector<Cycloid> cycles;
Signal sig = Signal();
std::deque<sf::Vector2f> epicyclePath;
std::vector<std::vector<sf::Vector2f>> userPaths;

ViewportHandler* ViewportHandler ::instancePtr = nullptr;
ViewportHandler* v = ViewportHandler::getInstance();

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
double fps = -1.0;

void compute(Point origin){
    std::vector<complex> points = sig.getComplex();
    reverse(points.begin(), points.end());

    Fourier f = Fourier(points, origin);
    f.DFT();
    f.sortRes();
    cycles = f.constructEpicycles();
    if (!cycles.empty()){
        drawing = true;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "DFT vectors");
//    sf::RenderWindow window(sf::VideoMode(2880, 1800), "DFT Visualizer");
    window.setVerticalSyncEnabled(true); // syncs application refresh rate to vertical freq. of monitor

    sf::Font font;
    if (!font.loadFromFile("../font/font.ttf"))
    {
        throw std::invalid_argument("Can't load font!");
    }

    v -> setState(sf::Vector2f(window.getSize()));

    sf::Clock clock;

    Menu m(sf::Vector2f(50, 50));

    while (window.isOpen())
    {
        sf::Event event;

        start = std::chrono::high_resolution_clock::now();

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
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (abs(event.mouseWheel.delta) >= 1 && tracking){
                    v -> setZoom( v->getZoom() + event.mouseWheel.delta*0.1);
                }
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::C){
                    epicyclePath.clear();
                    userPaths.clear();
                    cycles.clear();
                    sig.clear();
                    drawing = false;
                    tracking = false;
                    v -> setCenter(sf::Vector2f(0,0)); // center relative to origin
                    v -> setZoom(1);
                }
                if (event.key.code == sf::Keyboard::D){
                    compute(Point(v -> getOrigin()));
                    epicyclePath.clear();
                }
                if (event.key.code == sf::Keyboard::A){
                    maxCoef--;
                }
                if (event.key.code == sf::Keyboard::S){
                    maxCoef++;
                }
                if (event.key.code == sf::Keyboard::Z){
                    speedMulti -= 0.02;
                    epicyclePath.clear(); // prevent jumping
                }
                if (event.key.code == sf::Keyboard::X){
                    speedMulti += 0.02;
                    epicyclePath.clear(); // prevent jumping
                }
                if (event.key.code == sf::Keyboard::T){
                    tracking = !tracking;
                }
            }
        }

        if (!clicking){
            if (userPaths.size() == 0){
                userPaths.push_back(std::vector<sf::Vector2f>());
            }
            if (userPaths.back().size() > 0){
                userPaths.push_back(std::vector<sf::Vector2f>());
            }
        }

        if (clicking){
            v -> setMouse(sf::Vector2f(sf::Mouse::getPosition(window)));
            sig.addPoint(v -> getAbsoluteMousePos());
            if (userPaths.size()){
                userPaths.back().push_back(sf::Vector2f(sf::Mouse::getPosition(window)));
            }

        }

        window.clear(sf::Color::Black);

        /*------Text------*/
//        std::vector<sf::Text> texts;
//        sf::Text computeFourier("Compute epicycles (D)", font, 15);
//        sf::Text clearText("Clear text (C)", font, 15);
//        sf::Text epicycleNumText("Number of epicycles (A: -1, S: +1): " + std::to_string(maxCoef), font, 15);
//        sf::Text speedMultiText("Speed multiplier (Z: -0.02, X: +0.02): " + std::to_string(speedMulti).substr(0, 5), font, 15); // diplay up to third decimal
//        sf::Text msg("Dropdown in progress", font, 15);
//
//        texts.push_back(computeFourier);
//        texts.push_back(clearText);
//        texts.push_back(epicycleNumText);
//        texts.push_back(speedMultiText);
//        texts.push_back(msg);
//
//
//        for (int i=0; i<texts.size(); i++){
//            texts[i].setFillColor(sf::Color::White);
//            texts[i].setPosition(10.0f, 10.0f + 25.0f*(float)i);
//            window.draw(texts[i]);
//        }

        /*------Menu------*/

        m.draw(window, sf::RenderStates());

        /*------FPS------*/
        sf::Text fpsText("FPS " + std::to_string(fps).substr(0, 4), font, 15);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(window.getSize().x-100, 10);
        window.draw(fpsText);



        /*------Drawing User Path------*/
        for (std::vector<sf::Vector2f> &userP : userPaths){
            sf::RenderStates r;
            r.transform = v -> getTransform();
            if (userP.size() < 2){
                continue;
            }
            for (size_t i=1; i<userP.size(); ++i){
                sfLine l(userP[i-1], userP[i], sf::Color::Red, lineThickness/v->getZoom());
                l.draw(window, r);
            }
        }

        /*------Drawing Epicycles------*/
        if (drawing){
            Point pos = cycles[0].getPos();

            for (size_t i=0; i<cycles.size() && i < maxCoef; ++i){
                cycles[i].update(clock.getElapsedTime(), pos);
                pos = cycles[i].getEndPoint();
            }

            if (tracking){
                v -> setCenter(sf::Vector2f(pos.x - window.getSize().x*0.5f, pos.y - window.getSize().y*0.5f)); // center relative to origin
            }else{
                v -> setCenter(sf::Vector2f(0,0)); // center relative to origin
                v -> setZoom(1);
            }

            while (epicyclePath.size() > (double)maxPoints/speedMulti){
                epicyclePath.pop_front();
            }

            epicyclePath.push_back(sf::Vector2f(pos));

            for (size_t i=1; i<epicyclePath.size(); ++i){
                sfLine l(epicyclePath[i-1], epicyclePath[i], sf::Color::Blue, lineThickness/v->getZoom());
                sf::RenderStates r;
                r.transform = v -> getTransform();
                l.draw(window, r);
            }

            for (size_t i=0; i<cycles.size() && i < maxCoef; ++i){
                sf::RenderStates r;
                r.transform = v -> getTransform();
                cycles[i].draw(window, r);
            }
        }

        end = std::chrono::high_resolution_clock::now();
        fps = (fps*4.0f +(double)1e9/(double)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count())/5.0f; // weighted avg

        window.display();


    }

    return 0;
}
