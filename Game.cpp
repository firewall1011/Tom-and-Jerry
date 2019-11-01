#include <vector>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cmath>
#include <GL/glut.h>

#include "Vector2.h"
#include "Entity.h"

using namespace std;

/*Matrix defines*/
#define WIDTH 128
#define HEIGHT 128
#define EMPTY_SPACE ' '
#define CAT 'C'
#define MOUSE 'M'
#define FOOD 'F'

/*Genetic Algorithm defines*/
#define INITIAL_MOUSE_POPULATION 15
#define INITIAL_CAT_POPULATION 4
#define MAX_FOOD 40

// variables of entities
vector<Cat> cats;
vector<Mouse> mice;
vector<Food> foods;

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
    if( v.x + pos.x > 0 && v.y + pos.y > 0 && v.x + pos.x < (WIDTH - 1) && v.y + pos.y < (HEIGHT - 1)){
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
    getchar();
}

void drawEntities(void) {
    glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5.0f);

    glBegin(GL_POINTS);
        
        for (Food f : foods) {
            glColor3f(1.0f, 222.0f/255, 4.0f/255);
            glVertex2f(((((float) f.pos.x) / WIDTH) * 2) - 1.0f, 
                        ((((float) f.pos.y) / HEIGHT) * 2) - 1.0f);
        }

        for (Cat c : cats) {
            glColor3f(0.4f, 0.4f, 0.4f);
            glVertex2f(((((float) c.pos.x) / WIDTH) * 2) - 1.0f, 
                        ((((float) c.pos.y) / HEIGHT) * 2) - 1.0f);
        }

        for (Mouse m : mice) {
            glColor3f(90.0f/255, 50.0f/255, 26.0f/255);
            glVertex2f(((((float) m.pos.x) / WIDTH) * 2) - 1.0f, 
                        ((((float) m.pos.y) / HEIGHT) * 2) - 1.0f);
        }

    glEnd();

    glutSwapBuffers();
}

void init_pop() {
    for(int i = 0; i < INITIAL_CAT_POPULATION; i++){
        Cat cat = Cat();
        cat.pos.x = WIDTH-2;
        cat.pos.y = HEIGHT-2;
        cats.push_back(cat);
        // gridMap[(int)cat.pos.x][(int)cat.pos.y] = CAT;
    }

    for(int i = 0; i < INITIAL_MOUSE_POPULATION; i++){
        Mouse mouse = Mouse();
        mouse.pos.x = (int)(WIDTH/2);
        mouse.pos.y = (int)(HEIGHT/2);
        mice.push_back(mouse);
        // gridMap[(int)mouse.pos.x][(int)mouse.pos.y] = MOUSE;
    }

    for(int i = 0; i < MAX_FOOD; i++){
        Food f = Food();
        f.pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(f);
        // gridMap[(int)f.pos.x][(int)f.pos.y] = FOOD;
    }
}

void loop(int) {
    for(Mouse& m : mice){
        m.CheckRadar(cats, foods);
        m.move();
    }
    
    for(Cat& c : cats){
        c.CheckRadar(mice);
        c.move();
    }

    glutPostRedisplay();

    glutTimerFunc(1000/60, loop, 0);
}

int main(int argc, char* argv[]){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(640, 640);
    glutCreateWindow("Tom&Jerry");

    // char** gridMap = (char**)malloc(WIDTH * sizeof(char*));
    // for(int i = 0; i < WIDTH; i++) gridMap[i] = (char*)malloc(HEIGHT * sizeof(char));

    // for(int i = 0; i < WIDTH; i++)
    //     for(int j = 0; j < HEIGHT; j++)
    //         gridMap[i][j] = EMPTY_SPACE;

    srand(time(NULL));
    
    init_pop();

    // int num_loops = 1000;
    // //Game Loop
    // while(num_loops > 0){
    //     PrintMap(gridMap, WIDTH, HEIGHT);

    glutDisplayFunc(drawEntities);
    glutPostRedisplay();
    glutTimerFunc(0, loop, 0);

    glutMainLoop();
    // }
    // PrintMap(gridMap, WIDTH, HEIGHT);
    
}  