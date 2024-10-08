#include <SFML/Graphics.hpp>

#ifndef CONSTS_H
#define CONSTS_H

extern double speedMulti;

struct Consts {
    static constexpr double PI = 3.1415926535897932384626;

    static const int pointMulti = 10;

    static const int secToMilli = 1000;

    constexpr static const float minZoom = 0.1;
};

#endif