#pragma once

#include <SFML/Graphics.hpp>
#include <Consts.hpp>

struct ViewState{
    sf::Vector2f origin;
    float zoom;
    sf::Vector2f center;

    sf::Vector2f mousePos;
    sf::Vector2f mouseAbsPos;

    sf::Transform t;

    ViewState(){}

    ViewState(sf::Vector2f viewSize, const float z = (float)1) : origin(viewSize.x * 0.5f, viewSize.y * 0.5f), zoom(z), center(sf::Vector2f(0,0))
    {}

    void update(){
        t = sf::Transform::Identity;
        t.translate(origin);
        t.scale(zoom, zoom);
        t.translate(-origin);
        t.translate(-center);

    }

    void updateMouse(sf::Vector2f pos){
        mousePos = pos;
        mouseAbsPos = center + (pos - origin)/zoom;
    }

    sf::Vector2f convert(sf::Vector2f pos) const{
        return (center + (pos - origin)/zoom);
    }
};

class ViewportHandler{
private:
    static ViewportHandler* instancePtr;

public:
    ViewState state;

    ViewportHandler(){}

    ViewportHandler(const ViewportHandler& obj)=delete;

    static ViewportHandler* getInstance(){
        if (instancePtr == NULL){
            instancePtr = new ViewportHandler();
            return instancePtr;
        }else{
            return instancePtr;
        }
    }

    void setState(sf::Vector2f viewSize){
        this->state = ViewState(viewSize);
        this->state.update();
    }

    void setZoom(float z){
        if (z < Consts::minZoom){
            return;
        }
        this->state.zoom = z;
        this->state.update();
    }

    void setCenter(sf::Vector2f off){
        this->state.center = off;
        this->state.update();
    }

    void setOrigin(sf::Vector2f org){
        this->state.origin = org;
        this->state.update();
    }

    void setMouse(sf::Vector2f m){
        this->state.updateMouse(m);
    }

    sf::Vector2f getAbsoluteMousePos(){
        return this->state.mouseAbsPos;
    }

    void reset(){
        this->state.zoom = 1.0f;
        this->state.center = sf::Vector2f(0,0);
        this->state.update();
    }

    sf::Transform getTransform(){
        return this->state.t;
    }

    sf::Vector2f getOrigin(){
        return this->state.origin;
    }

    float getZoom(){
        return this->state.zoom;
    }

    sf::Vector2f convert(sf::Vector2f pos){
        return this->state.convert(pos);
    }
};