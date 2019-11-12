#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Entity.h"
#include <vector>

// /* Genetic Algorithm defines */
// #define INITIAL_MOUSE_POPULATION 20
// #define INITIAL_CAT_POPULATION 5
// #define MAX_FOOD 50

class GameManager{
    private:
    float mutation_rate;
    float speed_mutation;
    float smell_range_mutation;
    
    public:
    GameManager(float mut_rate = 0.1f, float sp_m = 1.0f, float sm_m = 0.5f){
        mutation_rate = mut_rate;
        speed_mutation = sp_m;
        smell_range_mutation = sm_m;
    }

    Cat reproduct(Cat& p1, Cat& p2){
        Cat child = Cat();
        child.smell_range = (p1.smell_range + p2.smell_range)/2;
        child.speed = (p1.speed + p2.speed)/2;

        mutate(child);
    }

    void mutate(Cat& c){
        int m = 1/mutation_rate;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? speed_mutation  : -speed_mutation) : 0;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? smell_range_mutation  : -smell_range_mutation) : 0;
    }

    Mouse reproduct(Mouse& p1, Mouse& p2){
        Mouse child = Mouse();
        child.smell_range = (p1.smell_range + p2.smell_range)/2;
        child.speed = (p1.speed + p2.speed)/2;

        mutate(child);
    }

    void mutate(Mouse& c){
        int m = 1/mutation_rate;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? speed_mutation  : -speed_mutation) : 0;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? smell_range_mutation  : -smell_range_mutation) : 0;
    }
};

#endif