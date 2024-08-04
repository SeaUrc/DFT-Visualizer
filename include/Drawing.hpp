#include <SFML/Graphics.hpp>

#include <Consts.hpp>
#include <Point.hpp>
#include <Lines.hpp>
#include <ViewportHandler.hpp>

struct Cycloid {

    ViewportHandler* vdraw = ViewportHandler::getInstance();

    Cycloid(float rad, Point pos, const int phi, float phase) {
        m_radius = rad;
        m_phi = phi;
        m_phase = phase;
        m_angle = phase;
        m_position = pos;

        trace.setRadius(rad);
        trace.setPointCount(rad*Consts::pointMulti);
        trace.setPosition(sf::Vector2f(pos + Point(-rad, -rad)));
        trace.setFillColor(sf::Color::Transparent);
        trace.setOutlineColor(sf::Color::White);
        trace.setOutlineThickness(1/vdraw->getZoom());

        arrow.setPrimitiveType(sf::Lines);
        generateArrow(pos, getEndPoint());

    }

    void setRadius(float radius) {
        m_radius = radius;
    }

    void setPosition(const Point position) {
        m_position = position;
    }

    Point getPos(){
        return m_position;
    }

    float getRadius(){
        return m_radius;
    }

    float getAngle(){
        return m_angle;
    }

    float getFreq(){
        return m_phi;
    }

    Point getEndPoint(){
        return Point(m_position.x + cos(m_angle) * m_radius, m_position.y + sin(m_angle) * m_radius);
    }

    void generateArrow(Point start, Point end) {
        arrow.clear();
        arrow.append(sf::Vertex(sf::Vector2f(start), sf::Color::White));
        arrow.append(sf::Vertex(sf::Vector2f(end), sf::Color::White));

        PolarPoint p1((end - start) * 0.3);
        p1.updateTheta(p1.th + Consts::PI*3/4); // 135 deg
        Point leftHead = p1.getPoint() + end;
        p1.updateTheta(p1.th + Consts::PI/2); // another 90
        Point rightHead = p1.getPoint() + end;

        arrow.append(sf::Vertex(sf::Vector2f(end), sf::Color::White));
        arrow.append(sf::Vertex(sf::Vector2f(leftHead), sf::Color::White));
        arrow.append(sf::Vertex(sf::Vector2f(end),  sf::Color::White));
        arrow.append(sf::Vertex(sf::Vector2f(rightHead), sf::Color::White));
    }

    void update(sf::Time t, Point pos){
        setPosition(pos);
        trace.setPosition(sf::Vector2f(pos + Point(-m_radius, -m_radius)));

        trace.setOutlineThickness(1/vdraw->getZoom());

        long long ms = t.asMilliseconds() * speedMulti;
        ms %= (long long)Consts::secToMilli * m_phi;
        m_angle = ((double)ms / (double)Consts::secToMilli * m_phi) * 2 * Consts::PI + m_phase;
        generateArrow(m_position, getEndPoint());
    }

    void draw(sf::RenderWindow &window, sf::RenderStates &r){

        window.draw(arrow, r);
        window.draw(trace, r);
    }

    private:
        float m_radius;
        int m_phi; // number of rotations in 1 second
        float m_phase;
        float m_angle;
        Point m_position;
        sf::CircleShape trace;
        sf::VertexArray arrow;
        sfLine line;
};

std::ostream& operator<<(std::ostream &ostream, Cycloid &p) {
    std::cout << "Cycloid(pos:" << p.getPos() << ", rad:" << p.getRadius() << ", angle:" << p.getAngle() << ", freq:" << p.getFreq() << ")";
    return ostream;
}
