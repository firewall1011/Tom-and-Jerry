#ifndef _ENTITY_
#define _ENTITY_

#define DEBUG 0
#include "Vector2.h"
#include <bits/stdc++.h>

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
        tracked_id = -1;
        tracked_pos = Vector2();
        representation = ' ';
    }
     friend bool operator==(const Entity& vA, const Entity& vB){std::cout << vA.pos << vB.pos << std::endl; return vA.pos == vB.pos;}
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
    void CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food);
};

class Cat : public Entity{
public:
    Cat(){
        setRepresentation('C');
    }
    void CheckRadar(std::vector<Mouse>& mice);
};

class Food : public Entity{
};

void Mouse::CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food){
    // current_state = Wandering;
    // double tracked_dist = smell_range;
    // // for(Cat& e : cats){
    // //     if(pos.distance(e.pos) <= tracked_dist){
    // //         current_state = RunningFrom;
    // //         tracked_dist = pos.distance(e.pos);
    // //         tracked = &e;
    // //         std::cout << "Cat Tracked: " << tracked->pos << tracked_dist << std::endl;
    // //     }
    // // }
    // // for(Food& f : food){
    // //     if(pos.distance(f.pos) <= tracked_dist){
    // //         current_state = RunningTo;
    // //         tracked_dist = pos.distance(f.pos);
    // //         tracked = &f;
    // //         std::cout << "Food Tracked: " << tracked->pos << tracked_dist << std::endl;
    // //     }
    // // }
    // if isnt wandering, check if has eaten mouse
    if (current_state == RunningTo && pos == tracked_pos) {
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
    if (current_state == RunningTo && pos == tracked_pos) {
        if (mice.size() > tracked_id && mice[tracked_id].pos == tracked_pos)
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
                std::cout << "Mouse Tracked: " << mice[tracked_id].pos << tracked_dist << std::endl;
        }
    }
}

#endif