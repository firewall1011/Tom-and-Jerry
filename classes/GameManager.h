#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
// /* Genetic Algorithm defines */
// #define INITIAL_MOUSE_POPULATION 20
// #define INITIAL_CAT_POPULATION 5
// #define MAX_FOOD 50

// MAYBE GAME MANAGER CONTAINS INITPOP & STUFF.

namespace GameManager{
    extern int mutation_rate;
    extern float speed_mutation;
    extern float smell_range_mutation;
    //extern vector<Cat*> cats;
    //extern vector<Mouse*> mice;
    //extern vector<Food*> foods;
    //void initPop()
    //void makeStep()
    //stuff of the game
    Cat* reproduct(Cat*, Cat*);
    Mouse* reproduct(Mouse*, Mouse*);
    void mutate(Cat* c);
    void mutate(Mouse* c);
};

#endif