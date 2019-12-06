#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
// /* Genetic Algorithm defines */
// #define INITIAL_MOUSE_POPULATION 20
// #define INITIAL_CAT_POPULATION 5
// #define MAX_FOOD 50

// MAYBE GAME MANAGER CONTAINS INITPOP & STUFF.


#define RAND_SEED 1337

/* Matrix defines */
#define WIDTH 128
#define HEIGHT 128

/* Rendering defines */
#define STEPS_PER_RENDER 100
#define RENDERS_PER_SEC 30


#include <vector>
#include <cmath>
#include <chrono>
#include <ctime>

#include "Vector2.h"
#include "Entity.h"
#include "Mouse.h"
#include "Cat.h"
#include "Food.h"
#include "GameManager.h"

namespace GameManager{
     using namespace std;

    /* Declaration of arrays */
    extern vector<Cat*> cats;
    extern vector<Mouse*> mice;
    extern vector<Food*> foods;

    Cat* reproduct(Cat*, Cat*);
    Mouse* reproduct(Mouse*, Mouse*);
    void mutate(Cat* c);
    void mutate(Mouse* c);
    void initPop();
    void makeStep();
    void makeNewGen();
    void genFood();
    bool genEnded();
};

#endif