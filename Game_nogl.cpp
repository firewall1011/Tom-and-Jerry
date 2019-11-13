#include <vector>
#include <cmath>
#include <string>
#include <cstring>

#include "classes/Vector2.h"
#include "classes/Entity.h"
#include "classes/Mouse.h"
#include "classes/Cat.h"
#include "classes/Food.h"

using namespace std;

/* Matrix defines */
#define WIDTH 128
#define HEIGHT 128

/* Genetic Algorithm defines */
#define INITIAL_MOUSE_POPULATION 2
#define INITIAL_CAT_POPULATION 0
#define MAX_FOOD 0

/* Rendering defines */
#define STEPS_PER_RENDER 1 //TODO: MUDAR PRA 1
#define RENDERS_PER_SEC 30

/* Declaration of arrays */
vector<Cat> cats;
vector<Mouse> mice;
vector<Food> foods;

void initPop() {
    for(int i = 0; i < INITIAL_CAT_POPULATION; i++){
        Cat cat = Cat();
        cat.pos.x = WIDTH-2;
        cat.pos.y = HEIGHT-2;
        cats.push_back(cat);
    }

    for(int i = 0; i < INITIAL_MOUSE_POPULATION; i++){
        Mouse mouse = Mouse();
        mouse.pos.x = (int)(WIDTH/2);
        mouse.pos.y = (int)(HEIGHT/2);
        mice.push_back(mouse);
    }

    for(int i = 0; i < MAX_FOOD; i++){
        Food f = Food();
        f.pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(f);
    }
}

void makeStep() {
    for(int i = 0; i < mice.size(); i++){ // Maybe read from size to 0, or push child in front not back
        Mouse& m = mice[i];

        cout<<"Mice with id" << i << "doing his thing" << endl;
        m.calculateReproductionUrge(1, 1, mice.size());
        m.CheckRadar(cats, foods, mice);
        m.move(WIDTH, HEIGHT);

        // if is dead (energy = 0), delete
        if(m.energyConsume()){
            mice.erase(mice.begin() + i);
            i--;
            printf("Mouse died hunger\nPopulation: %lu\n", mice.size());            
        }
    }
    
    for(int i = 0; i < cats.size(); i++){
        Cat& c = cats[i];

        c.calculateReproductionUrge(1, 1, cats.size());
        c.CheckRadar(mice, cats);
        c.move(WIDTH, HEIGHT);

        // if is dead (energy = 0), delete
        if(c.energyConsume()){
            cats.erase(cats.begin() + i);
            i--;
            printf("Cat died hunger\nPopulation: %lu\n", cats.size());  
        }
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    initPop();
    char c = 'a';
    while(c != 's'){
        for (int i = 0; i < STEPS_PER_RENDER; i++)
            makeStep();
        c = getchar();
    }
}  