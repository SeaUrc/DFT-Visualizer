#include <complex>
#include <Point.hpp>

using complex = std::complex<double>;

class Signal{

public:
    void addPoint(Point p){
        points.push_back(p);
    }

    size_t size(){
        return points.size();
    }

    std::vector<complex> getComplex(){
        std::vector<complex> res(points.size());
        for (size_t i=0; i<points.size(); ++i){
            res[i] = complex(points[i]);
        }
        return res;
    }

    void clear(){
        points.clear();
    }

    std::vector<sf::Vector2f> getVector2f(){
        std::vector<sf::Vector2f> res(points.size());
        for (size_t i=0; i<points.size(); ++i){
            res[i] = sf::Vector2f(points[i].x, points[i].y);
        }
        return res;
    }

    void setPoints(std::vector<Point> p){
        points = p;
    }

    Signal(){

    }

private:
    std::vector<Point> points;

};