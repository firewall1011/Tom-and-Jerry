#include <vector>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cmath>

#include "Vector2.h"
#include "Entity.h"

using namespace std;

/*Matrix defines*/
#define WIDTH 32
#define HEIGHT 32
#define EMPTY_SPACE ' '
#define CAT 'C'
#define MOUSE 'M'
#define FOOD 'F'

/*Genetic Algorithm defines*/
#define INITIAL_MOUSE_POPULATION 3
#define INITIAL_CAT_POPULATION 2
#define MAX_FOOD 2

void Entity::move(){
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
        //Call runningFrom technique
        v = (tracked_pos - pos).normal();
    }
    if( v.x + pos.x >= 0 && v.y + pos.y >= 0 && v.x + pos.x < (WIDTH - 1) && v.y + pos.y < (HEIGHT - 1)){
        v += pos;
    }
    else{
        v -= pos;
    }
    if(v.x >= 0 && v.y >= 0 && v.x < WIDTH && v.y < HEIGHT) pos = v;
    pos.x = round(pos.x); pos.y = round(pos.y);
}

void PrintMap(char** grid, int w, int h){
    // sls("clear");
    for(int i = -1; i <= w; i++){
        for(int j = -1; j <= h; j++){
            if(j == -1 || j == w) printf("|");
            else if(i == -1 || i == h) printf(" =");
            else printf(" %c", grid[i][j]);
        }
        printf("\n");
    }
}

int main(){
    char** gridMap = (char**)malloc(WIDTH * sizeof(char*));
    for(int i = 0; i < WIDTH; i++) gridMap[i] = (char*)malloc(HEIGHT * sizeof(char));

    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < HEIGHT; j++)
            gridMap[i][j] = EMPTY_SPACE;

    srand(time(NULL));

    vector<Cat> cats;
    vector<Mouse> mice;
    vector<Entity> entities;

    for(int i = 0; i < INITIAL_CAT_POPULATION; i++){
        Cat cat = Cat();
        cat.pos.x = WIDTH-1;
        cat.pos.y = HEIGHT-1;
        cats.push_back(cat);
        entities.push_back(cat);
        gridMap[(int)cat.pos.x][(int)cat.pos.y] = CAT;
    }

    for(int i = 0; i < INITIAL_MOUSE_POPULATION; i++){
        Mouse mouse = Mouse();
        mouse.pos.x = (int)(WIDTH/2);
        mouse.pos.y = (int)(HEIGHT/2);
        mice.push_back(mouse);
        entities.push_back(mouse);
        gridMap[(int)mouse.pos.x][(int)mouse.pos.y] = MOUSE;
    }


    vector<Food> foods;
    for(int i = 0; i < MAX_FOOD; i++){
        Food f = Food();
        f.pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(f);
        entities.push_back(f);
        gridMap[(int)f.pos.x][(int)f.pos.y] = FOOD;
    }

    int num_loops = 1000;
    //Game Loop
    while(num_loops > 0){
        PrintMap(gridMap, WIDTH, HEIGHT);
        
        for(Mouse& e : mice){
            gridMap[(int)e.pos.x][(int)e.pos.y] = EMPTY_SPACE;
            e.CheckRadar(cats, foods);
            e.move();
            gridMap[(int)e.pos.x][(int)e.pos.y] = e.getRepresentation();
        }
        
        for(Cat& e : cats){
            gridMap[(int)e.pos.x][(int)e.pos.y] = EMPTY_SPACE;
            e.CheckRadar(mice);
            e.move();
            gridMap[(int)e.pos.x][(int)e.pos.y] = e.getRepresentation();
        }

        for(Food f : foods){
            gridMap[(int)f.pos.x][(int)f.pos.y] = f.getRepresentation();
        }
        
        num_loops--;
    }
    PrintMap(gridMap, WIDTH, HEIGHT);

}