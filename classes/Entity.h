#ifndef ENTITY_H
#define ENTITY_H

#include "Vector2.h"
#include "State.h"

#include <chrono>

class Entity{
//Attributes
public: 
    int smell_range;
    int tracked_id;
    int childhood;
    float speed;
    float energy;
    float reproduction_limiar;
    int reproduction_amount;
    float reproduction_urge;
    float atractiveness;
    std::chrono::system_clock::time_point spawnTime;
    state current_state;
    Vector2 pos;
    Vector2 facing_dir;
    Entity* tracked;
private:
    float *representation;
public:
    //Constructor
    Entity(int sr, float s, float rl);

    // operators
    friend bool operator==(const Entity& vA, const Entity& vB){return vA.pos == vB.pos;}
    friend bool operator!=(const Entity& vA, const Entity& vB){return vA.pos != vB.pos;}

    //Getters and Setters
    float* getRepresentation() { return this->representation; }
    void setRepresentation(float* r) {this->representation = r;}

    //Methods
    void move(int w, int h);
    void draw(int w, int h);
    bool energyConsume();
    void addEnergy(float amount);
    void calculateReproductionUrge(int E, int N);
    virtual void CheckRadar(){}
};

#endif