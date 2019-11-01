#ifndef _ENTITY_
#define _ENTITY_

#include "Vector2.h"
#include <vector>

enum state{
    Wandering, RunningFrom, RunningTo, Dead
};

class Entity;
class Mouse;
class Cat;
class Food;

class Entity{
//Attributes
public: 
    int smell_range;
    float speed;
    float hunger;
    float reproduction_urge;
    float atractiveness;
    state current_state;
    Vector2 pos;
    Vector2 tracked;
private:
    char representation;
//Constructor
public:
    Entity(){ 
        smell_range = 5;
        speed = 1;
        hunger = 0.0;
        reproduction_urge = 0.0;
        atractiveness = 0.0;
        current_state = Wandering;
        pos = Vector2();
        tracked = Vector2();
        representation = ' ';
    }
//Getters and Setters
    char getRepresentation() { return this->representation; }
    void setRepresentation(char r) {this->representation = r;}
//Methods
    void Move();
    void CalculateHunger();
    bool isDead(){ return current_state == Dead; }
    virtual void CheckRadar(){}
};

class Mouse : public Entity{
public:
    Mouse(){
        setRepresentation('M');
    }
    void CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food);
};

class Cat : public Entity{
public:
    Cat(){
        setRepresentation('C');
    }
    void CheckRadar(std::vector<Mouse>& mouses);
};

class Food : public Entity{
public:
    Food(){
        setRepresentation('F');
    }
    void CheckRadar(){
        return;
    }
};

void Mouse::CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food){
    if(isDead()) return;
    tracked = Vector2();
    current_state = Wandering;
    double tracked_dist = smell_range;
    for(Cat& e : cats){
        if(e.current_state != Dead && pos.distance(e.pos) <= tracked_dist){
            current_state = RunningFrom;
            tracked_dist = pos.distance(e.pos);
            tracked = (e.pos);
            if(tracked_dist < 1) current_state = Dead;
            //std::cout << "Cat Tracked: " << tracked << tracked_dist << std::endl;
        }
    }
    for(Food& f : food){
        if(f.current_state != Dead && pos.distance(f.pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(f.pos);
            tracked = (f.pos);
            if(tracked_dist < 1){
                hunger = 0;
                f.current_state = Dead;
            }
            std::cout << "Food Tracked: " << tracked << tracked_dist << std::endl;
        }
    }
}

void Cat::CheckRadar(std::vector<Mouse>& mouses){
    if(isDead()) return;
    tracked = Vector2();
    current_state = Wandering;
    double tracked_dist = smell_range;
    for(Mouse& e : mouses){
        if(e.current_state != Dead && pos.distance(e.pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(e.pos);
            tracked = (e.pos);
            if(tracked_dist < 1){
                hunger = 0;
                e.current_state = Dead;
            }
            std::cout << "Mouse Tracked: " << tracked << tracked_dist << std::endl;
        }
    }
}

#endif