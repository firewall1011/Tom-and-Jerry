#ifndef ENTITY_H
#define ENTITY_H

#include "Vector2.h"
#include "State.h"

class Entity{
//Attributes
public: 
    int smell_range;
    float speed;
    float energy;
    float reproduction_urge;
    float atractiveness;
    state current_state;
    Vector2 pos;
    int tracked_id;
    Vector2 tracked_pos;
private:
    float *representation;
public:
    //Constructor
    Entity();

    // operators
    friend bool operator==(const Entity& vA, const Entity& vB){std::cout << vA.pos << vB.pos << std::endl; return vA.pos == vB.pos;}

    //Getters and Setters
    float* getRepresentation() { return this->representation; }
    void setRepresentation(float* r) {this->representation = r;}

    //Methods
    void move(int w, int h);
    void draw(int w, int h);
    bool energyConsume();
    void energyRecover(float amount);
    virtual void CheckRadar(){}
};

#endif