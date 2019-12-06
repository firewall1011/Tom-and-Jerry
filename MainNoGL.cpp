#include "classes/GameManager.h"

#define STEPS_PER_RENDER 100

using namespace GameManager;


void runGame() {
    initPop();

    while(true)  {
        genFood();

        for (int i = 0; i < STEPS_PER_RENDER; i++)
            makeStep();

        if(genEnded()) makeNewGen();

    }
    return;
}

int main(void){
    srand(RAND_SEED);
    
    runGame();
}  