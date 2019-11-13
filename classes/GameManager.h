#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
// /* Genetic Algorithm defines */
// #define INITIAL_MOUSE_POPULATION 20
// #define INITIAL_CAT_POPULATION 5
// #define MAX_FOOD 50

namespace GameManager{
    extern float mutation_rate;
    extern float speed_mutation;
    extern float smell_range_mutation;
    Cat reproduct(Cat& p1, Cat& p2);
    Mouse reproduct(Mouse& p1, Mouse& p2);
    void mutate(Cat& c);
    void mutate(Mouse& c);
    void printChild(Mouse& e);
    void printChild(Cat& e);
};

#endif