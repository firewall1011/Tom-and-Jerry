#ifndef _ENTITY_
#define _ENTITY_

#define DEBUG 0
#include "Vector2.h"
#include <GL/glut.h>
#include <vector>
#include <iostream>

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
    int tracked_id;
    Vector2 tracked_pos;
private:
    float *representation;
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
        tracked_id = -1;
        tracked_pos = Vector2();
        representation = new (float [3]) {0.0, 0.0, 0.0};
    }
     friend bool operator==(const Entity& vA, const Entity& vB){std::cout << vA.pos << vB.pos << std::endl; return vA.pos == vB.pos;}
//Getters and Setters
    float* getRepresentation() { return this->representation; }
    void setRepresentation(float* r) {this->representation = r;}
//Methods
    void move(int w, int h);
    void draw(int w, int h);
    virtual void CheckRadar(){}
};

void Entity::move(int w, int h){
    Vector2 v;
    if(current_state == Wandering){
        //Call wandering technique
        v = Vector2((rand()%3 - 1), (rand()%3 - 1)) * speed;
    }
    else if(current_state == RunningFrom){
        //Call runningFrom technique
        v = (pos - tracked_pos).normal();
    }
    else if(current_state == RunningTo){
        //Call runningTo technique
        v = (tracked_pos - pos).normal();
    }

    v.roundInt();

    if( v.x + pos.x >= 0 && v.y + pos.y >= 0 && v.x + pos.x < w && v.y + pos.y < h){
        v += pos;
    }
    else{
        // explain this please! this is bugging me (/alex)
        v -= pos;
    }
    
    if(v.x >= 0 && v.y >= 0 && v.x < w && v.y < h) 
        pos = v;
}

void Entity::draw(int w, int h) {
    glColor3fv(getRepresentation());
    glVertex2f(((((float) pos.x) / w) * 2) - 1.0f, 
                ((((float) pos.y) / h) * 2) - 1.0f);
}

class Mouse : public Entity{
public:
    Mouse(){
        setRepresentation(new (float [3]) {90.0f/255, 50.0f/255, 26.0f/255});
    }
    void CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food);
};

class Cat : public Entity{
public:
    Cat(){
        setRepresentation(new (float [3]) {0.4f, 0.4f, 0.4f});
    }
    void CheckRadar(std::vector<Mouse>& mice);
};

class Food : public Entity{
public:
    Food(){
        setRepresentation(new (float [3]) {1.0f, 222.0f/255, 4.0f/255});
    }
};

void Mouse::CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food){
    // if isnt wandering, check if has eaten food
    if (current_state == RunningTo && pos.distance(tracked_pos) < 1) {
        if (food.size() > tracked_id && food[tracked_id].pos == tracked_pos)
            food.erase(food.begin() + tracked_id);
        tracked_id = -1;
        tracked_pos = Vector2();
    }

    // DEBUG print of all food
    if (DEBUG) {
        for(Food f : food)
            std::cout << f.getRepresentation() << f.pos;
        std::cout << std::endl;

        for(Cat c : cats)
            std::cout << c.getRepresentation() << c.pos;
        std::cout << std::endl;

    }
    
    // change to Wandering, in case the food tracked is eaten, or ran away from cat
    current_state = Wandering;

    // look for the nearest cat to run away from, checking on smell_range
    double tracked_dist = smell_range;
    for(int i = 0; i < cats.size(); i++){
        Cat c = cats[i];
        if(pos.distance(c.pos) <= tracked_dist){
            current_state = RunningFrom;
            tracked_dist = pos.distance(c.pos);
            tracked_id = i;
            tracked_pos = c.pos;
            if (DEBUG)
                std::cout << "Cat Tracked: " << cats[tracked_id].pos << tracked_dist << std::endl;
        }
    }

    // check if could track a cat, and if is tracking, return 
    if (current_state == RunningFrom) return;

    // look for the nearest food to track, checking on smell_range
    // if can't track any food, keeps Wandering
    for(int i = 0; i < food.size(); i++){
        Food f = food[i];
        if(pos.distance(f.pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(f.pos);
            tracked_id = i;
            tracked_pos = f.pos;
            if (DEBUG)
                std::cout << "Food Tracked: " << food[tracked_id].pos << tracked_dist << std::endl;
        }
    }
}

void Cat::CheckRadar(std::vector<Mouse>& mice){
    // if isnt wandering, check if has eaten mouse
    if (current_state == RunningTo && pos.distance(tracked_pos) < 1) {
        if (mice.size() > tracked_id && mice[tracked_id].pos.distance(tracked_pos) < 1)
            mice.erase(mice.begin() + tracked_id);
        tracked_id = -1;
        tracked_pos = Vector2();
    }

    // DEBUG print of all mice
    if (DEBUG) {
        for(Mouse m : mice)
            std::cout << m.getRepresentation() << m.pos;
        std::cout << std::endl;
    }

    // change to Wandering to give a chance of mouse tracked run away
    current_state = Wandering;

    // look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    double tracked_dist = smell_range;
    for(int i = 0; i < mice.size(); i++){
        Mouse m = mice[i];
        if(pos.distance(m.pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(m.pos);
            tracked_id = i;
            tracked_pos = m.pos;
            if (DEBUG)
                std::cout << "Mouse Tracked: " << mice[tracked_id].pos << tracked_dist << (tracked_pos - pos) << std::endl;

        }
    }
}

#endif