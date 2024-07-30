#pragma once

#include <SFML/Graphics.hpp>

struct ViewState{
    sf::Vector2f origin;
    float zoom;
    sf::Vector2f center;

    sf::Vector2f mousePos;
    sf::Vector2f mouseAbsPos;

    sf::Transform t;

    ViewState(sf::Vector2f viewSize, const float z = (float)1) : origin(viewSize.x * 0.5f, viewSize.y * 0.5f), zoom(z), center(sf::Vector2f(0,0))
    {}

    void update(){
        t = sf::Transform::Identity;
        t.translate(origin);
        t.translate(-center*zoom);
        t.scale(zoom, zoom);
        t.translate(-origin);

    }

    void updateMouse(sf::Vector2f pos){
        mousePos = pos;
        mouseAbsPos = center + (pos - origin)/zoom;
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

    void setCenter(sf::Vector2f off){
        state.center = off;
        state.update();
    }

    void setOrigin(sf::Vector2f org){
        state.origin = org;
        state.update();
    }

    void setMouse(sf::Vector2f m){
        state.updateMouse(m);
    }

    sf::Vector2f getAbsoluteMousePos(){
        return state.mouseAbsPos;
    }

    void reset(){
        state.zoom = 1.0f;
        state.center = sf::Vector2f(0,0);
        state.update();
    }

    sf::Transform getTransform(){
        return state.t;
    }

    sf::Vector2f getOrigin(){
        return state.origin;
    }
};