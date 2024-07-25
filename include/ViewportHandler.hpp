#pragma once

#include <SFML/Graphics.hpp>

struct ViewState{
    sf::Vector2f origin;
    float zoom;
    sf::Vector2f offset;

    sf::Vector2f mousePos;
    sf::Vector2f mouseAbsPos;

    sf::Transform t;

    ViewState(sf::Vector2f viewSize, const float z = (float)1) : origin(viewSize.x * 0.5f, viewSize.y * 0.5f), zoom(z), offset(origin/zoom)
    {}

    void update(){
        t = sf::Transform::Identity;
        t.translate(origin);
        t.scale(zoom, zoom);
        t.translate(-offset);
    }

    void updateMouse(sf::Vector2f pos){
        mousePos = pos;
        mouseAbsPos = offset + (pos - center)/zoom;
    }
};

struct ViewportHandler{
    ViewState state;

    ViewportHandler(sf::Vector2f viewSize): state(viewSize)
    {
        state.update();
    }

    void setZoom(float z){
        state.zoom = z;
        state.update();
    }



    void reset(){
        state.zoom = 1.0f;

    }
};