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
    Entity(){ representation = ' '; current_state = Wandering; speed = 1;}
//Getters and Setters
    char getRepresentation() { return this->representation; }
    void setRepresentation(char r) {this->representation = r;}
//Methods
    void move();
    virtual void CheckRadar(){}
};

class Mouse : public Entity{
public:
    Mouse(){
        setRepresentation('M');
    }
    void CheckRadar(std::vector<Cat> cats, std::vector<Food> food){
        printf("Using Mouse rad\n");
    }
};

class Cat : public Entity{
public:
    Cat(){
        setRepresentation('C');
    }
    void CheckRadar(std::vector<Mouse> mouses){
        printf("Using Cat rad\n");
    }
};

class Food : public Entity{
};

#endif