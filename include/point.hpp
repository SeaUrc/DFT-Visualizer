#pragma once

#include <complex>
#include <SFML/graphics.hpp>
#include <iostream>

using complex = std::complex<double>;

struct Point {
    Point() : x(0.0), y(0.0) {
    }

    Point(double inX, double inY) : x(inX), y(inY){
    }

    operator complex() const {
        return complex(x, y);
    }

    operator sf::Vector2f() const {
        return sf::Vector2f(x, y);
    }

    void operator += (const Point& p){
        x += p.x;
        y += p.y;
    }

    Point operator + (const Point &other) const {
        return Point(x + other.x, y + other.y);
    }
    Point operator - (const Point &other) const{
        return Point (x - other.x, y - other.y);
    }
    Point operator * (float scalar) const {
        return Point(x * scalar, y * scalar);
    }
    Point operator / (float scalar) const {
        return Point(x / scalar, y / scalar);
    }

    double x, y;
};

std::ostream& operator<<(std::ostream &ostream, const Point &p) {
    std::cout << '(' << p.x << ", " << p.y << ')';
    return ostream;
}

struct PolarPoint {
    PolarPoint() : r(0.0), th(0.0) {
    }

    PolarPoint(double radius, double theta) : r(radius), th(theta) {

    }

    PolarPoint(const Point& p){
        r = sqrt(p.x * p.x + p.y * p.y);
        th = acos( p.x / r);
        if (p.y < 0){
            th *= -1.0;
        }
    }

    void updateRadius(double rad){
        r = rad;
    }

    void updateTheta(double theta){
        th = theta;
    }

    Point getPoint(){
        return Point(r * cos(th), r*sin(th));
    }

    double r, th;
};

std::ostream& operator<<(std::ostream &ostream, const PolarPoint &p) {
    std::cout << '(' << p.r << ", " << p.th << ')';
    return ostream;
}

double dist(const Point &p1, const Point &p2){
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

