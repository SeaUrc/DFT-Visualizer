#include <SFML/Graphics.hpp>
#include <Drawing.hpp>
#include <DFT.hpp>
#include <Signal.hpp>
#include <Lines.hpp>
#include <ViewportHandler.hpp>
#include <FileUpload.hpp>

#include <iostream>
#include <deque>

double speedMulti = 0.05;
int maxCoef = 10;
int maxPoints = 110;
float lineThickness = 1.5;

/*---states---*/
bool clicking = false;
bool tracking = false;
bool drawing = false;
bool drawingCircle = true;
bool shifting = false;

/*---paths---*/
std::vector<Cycloid> cycles;
Signal sig = Signal();
std::deque<sf::Vector2f> epicyclePath;
std::vector<std::vector<sf::Vector2f>> userPaths;

/*---viewport handler---*/
ViewportHandler *ViewportHandler::instancePtr = nullptr;
ViewportHandler *v = ViewportHandler::getInstance();

/*---fps related---*/
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
double fps = -1.0;

/*---compute DFT & update cycles---*/
void compute(Point origin) {
    std::vector<complex> points = sig.getComplex();
    reverse(points.begin(), points.end());
    Fourier f = Fourier(points, origin);
    f.DFT();
    cycles = f.constructEpicycles();
    Fourier::sortByFrequency(cycles);
    if (!cycles.empty()) {
        drawing = true;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "DFT vectors", sf::Style::Default);
    window.setVerticalSyncEnabled(true); // syncs application refresh rate to vertical freq. of monitor

    sf::Font font;
    if (!font.loadFromFile("/Users/nick/CLionProjects/DFT/font/font.ttf")) // upload font
    {
        throw std::invalid_argument("Can't load font!");
    }

    v->setState(sf::Vector2f(window.getSize())); // initalize viewport handler

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;

        start = std::chrono::high_resolution_clock::now();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                clicking = true;
            }
            if (event.type == sf::Event::MouseButtonReleased && clicking) {
                clicking = false;
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (abs(event.mouseWheel.delta) >= 1 && tracking) { // zooming
                    if (shifting) {
                        v->setZoom(v->getZoom() / pow(.95, event.mouseWheel.delta * 0.7));
                    } else {
                        v->setZoom(v->getZoom() / pow(.95, event.mouseWheel.delta * 0.1));
                    }

                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift) { // shift toggle
                    shifting = !shifting;
                }
                if (event.key.code == sf::Keyboard::C) { // clear path
                    epicyclePath.clear();
                    userPaths.clear();
                    cycles.clear();
                    sig.clear();
                    drawing = false;
                    tracking = false;
                    v->setCenter(sf::Vector2f(0, 0)); // center relative to origin
                    v->setZoom(1);
                }
                if (event.key.code == sf::Keyboard::D) { // compute DFT
                    compute(Point(v->getOrigin()));
                    epicyclePath.clear();
                }
                if (event.key.code == sf::Keyboard::A) { // number of coefficients
                    if (shifting) {
                        maxCoef -= 10;
                    } else {
                        maxCoef--;
                    }
                }
                if (event.key.code == sf::Keyboard::S) { // number of coefficients
                    if (shifting) {
                        maxCoef += 10;
                    } else {
                        maxCoef++;
                    }
                }
                if (event.key.code == sf::Keyboard::Z) { // simulation speed
                    if (shifting) {
                        speedMulti *= 0.6;
                    } else {
                        speedMulti *= 0.95;
                    }
                    epicyclePath.clear(); // prevent jumping
                }
                if (event.key.code == sf::Keyboard::X) { // simulation speed
                    if (shifting) {
                        speedMulti /= 0.6;
                    } else {
                        speedMulti /= 0.95;
                    }
                    epicyclePath.clear(); // prevent jumping
                }
                if (event.key.code == sf::Keyboard::T) { // tracking toggle
                    tracking = !tracking;
                }
                if (event.key.code == sf::Keyboard::L) { // circle toggle
                    drawingCircle = !drawingCircle;
                }
                if (event.key.code == sf::Keyboard::I) { // input file
                    std::string inputFile = openFileDialog();
                    std::vector<Point> inputPoints = extractPointsFromCSV(inputFile.c_str());
                    std::vector<Point> sigPoints = inputPoints;
                    for (Point &p: sigPoints) {
                        p = p - Point(v->getOrigin());
                    }
                    sig.setPoints(sigPoints);
                    std::vector<sf::Vector2f> inputVec2f(inputPoints.size());
                    for (size_t i = 0; i < inputPoints.size(); ++i) {
                        inputVec2f[i] = sf::Vector2f(inputPoints[i]);
                    }
                    userPaths.resize(1);
                    userPaths[0] = inputVec2f;
                }
            }
        }

        if (!clicking) {
            if (userPaths.empty()) {
                userPaths.emplace_back();
            }
            if (!userPaths.back().empty()) {
                userPaths.emplace_back();
            }
        }

        if (clicking) {
            v->setMouse(sf::Vector2f(sf::Mouse::getPosition(window)));
            sig.addPoint(v->getAbsoluteMousePos());
            if (!userPaths.empty()) {
                userPaths.back().emplace_back(sf::Mouse::getPosition(window));
            }
        }

        window.clear(sf::Color::Black);

        /*------Drawing User Path------*/
        for (std::vector<sf::Vector2f> &userP: userPaths) {
            sf::RenderStates r;
            r.transform = v->getTransform();
            if (userP.size() < 2) {
                continue;
            }
            for (size_t i = 1; i < userP.size(); ++i) {
                sfLine l(userP[i - 1], userP[i], sf::Color::Red, lineThickness / v->getZoom());
                l.draw(window, r);
            }
        }

        /*------Drawing------*/
        if (drawing) {

            /*---Sorting epicycles by radius---*/
            std::vector<Cycloid> radiusSortedEpicycles;
            for (size_t i = 0; i < cycles.size() && i < maxCoef; ++i) {
                radiusSortedEpicycles.push_back(cycles[i]);
            }

            Fourier::sortByRadius(radiusSortedEpicycles);

            /*---Updating epicycles---*/
            Point pos = v->getOrigin();

            for (size_t i = 0; i < radiusSortedEpicycles.size(); ++i) {
                radiusSortedEpicycles[i].update(clock.getElapsedTime(), pos);
                pos = radiusSortedEpicycles[i].getEndPoint();
            }

            /*---Setting viewport---*/
            if (tracking) {
                v->setCenter(sf::Vector2f(pos.x - window.getSize().x * 0.5f,
                                          pos.y - window.getSize().y * 0.5f)); // center relative to origin
            } else {
                v->setCenter(sf::Vector2f(0, 0)); // center relative to origin
                v->setZoom(1);
            }

            while (epicyclePath.size() > (double) maxPoints / speedMulti) {
                epicyclePath.pop_front();
            }

            epicyclePath.push_back(sf::Vector2f(pos));
            /*---Draw epicycles path---*/
            for (size_t i = 1; i < epicyclePath.size(); ++i) {
                sfLine l(epicyclePath[i - 1], epicyclePath[i], sf::Color::Blue, lineThickness / v->getZoom());
                sf::RenderStates r;
                r.transform = v->getTransform();
                l.draw(window, r);
            }

            /*---Draw epicycles---*/
            for (size_t i = 0; i < radiusSortedEpicycles.size() && i < maxCoef; ++i) {
                sf::RenderStates r;
                r.transform = v->getTransform();
                if (drawingCircle) {
                    radiusSortedEpicycles[i].draw(window, r);
                } else {
                    radiusSortedEpicycles[i].drawWithNoCircles(window, r);
                }
            }

        }

        /*------Text------*/
        std::vector<sf::Text> texts;
        sf::Text computeFourier("Compute epicycles (D)", font, 15);
        sf::Text clearText("Clear text (C)", font, 15);
        sf::Text epicycleNumText("Number of epicycles (A: -1 | S: +1) : " + std::to_string(maxCoef), font, 15);
        sf::Text speedMultiText("Speed multiplier (Z: *0.95 | X: /0.95): " + std::to_string(speedMulti).substr(0, 5),
                                font, 15); // diplay up to third decimal
        sf::Text zoomText("Zoom (scroll wheel, only when tracking): " + std::to_string(v->getZoom()).substr(0, 3), font,
                          15);
        std::string trackOn = (tracking ? "ON" : "OFF");
        sf::Text trackText("Camera track (T) " + trackOn, font, 15);
        std::string circleOn = (drawingCircle ? "ON" : "OFF");
        sf::Text showCircle("Toggle circles (L) " + circleOn, font, 15);
        sf::Text inputText("Load csv file (I)", font, 15);
        std::string multi = (shifting ? "ON" : "OFF");
        sf::Text shiftText("Toggle 10x multiplier on inputs (shift) " + multi, font, 15);

        texts.push_back(computeFourier);
        texts.push_back(clearText);
        texts.push_back(epicycleNumText);
        texts.push_back(speedMultiText);
        texts.push_back(zoomText);
        texts.push_back(trackText);
        texts.push_back(showCircle);
        texts.push_back(inputText);
        texts.push_back(shiftText);


        for (int i = 0; i < texts.size(); i++) {
            texts[i].setFillColor(sf::Color::White);
            texts[i].setPosition(10.0f, 10.0f + 25.0f * (float) i);
            window.draw(texts[i]);
        }


        /*------FPS------*/
        sf::Text fpsText("FPS " + std::to_string(fps).substr(0, 4), font, 15);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(window.getSize().x - 100, 10);
        window.draw(fpsText);

        end = std::chrono::high_resolution_clock::now();
        fps = (fps * 4.0f +
               (double) 1e9 / (double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) /
              5.0f; // weighted avg

        window.display();


    }

    return 0;
}
