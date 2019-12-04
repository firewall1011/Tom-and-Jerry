#include <vector>
#include <cmath>
//#include <GL/glut.h>
#include <string>
#include <cstring>

#include "classes/Vector2.h"
#include "classes/Entity.h"
#include "classes/Mouse.h"
#include "classes/Cat.h"
#include "classes/Food.h"

using namespace std;

// some functions could go to GameManager ns.
// these functions are marked right before its
// declarations.

/* Matrix defines */
#define WIDTH 128
#define HEIGHT 128

/* Rendering defines */
#define STEPS_PER_RENDER 1
#define RENDERS_PER_SEC 30

/* Genetic Algorithm declarations */
int score = 0;
//Cats params
int cat_population = 2;
int cat_smell_range = 5;
int cat_speed = 1;
float cat_reproduction_limiar = 0.6;

//Mice params
int mouse_population = 1000;
int mouse_smell_range = 5;
int mouse_speed = 1;
float mouse_reproduction_limiar = 0.6;
int food_amount = 50;
int food_spawn_difficulty  = 30;

// #define INITIAL_MOUSE_POPULATION 1000
// #define INITIAL_CAT_POPULATION 2
// #define INITIAL_FOOD_AMOUNT 50
// #define FOOD_SPAWN_RATE 30

/* Declaration of arrays */
vector<Cat*> cats;
vector<Mouse*> mice;
vector<Food*> foods;

void drawFoods() {
    for (Food* f : foods)
        f->draw(WIDTH, HEIGHT);  
}

void drawCats() {
    for (Cat* c : cats)
        c->draw(WIDTH, HEIGHT);
}

void drawMice() {
    for (Mouse* m : mice)
        m->draw(WIDTH, HEIGHT);
}

// void writeRates() {
//     glPushMatrix();

//     string ratestr = "running on ";
//     ratestr += to_string(STEPS_PER_RENDER * RENDERS_PER_SEC);
//     ratestr += " steps/sec";

//     unsigned char rate[ratestr.size() + 1];
//     strcpy((char*) rate, ratestr.c_str());

//     glColor3f(0.0, 0.0, 0.0);
//     glRasterPos2f(-0.9, 0.9);

//     int len = strlen((char*) rate);
//     for (int i = 0; i < len; i++) {
//         glutBitmapCharacter(GLUT_BITMAP_8_BY_13, rate[i]);
//     }

//     glPopMatrix();
// }

// void writeStats() {
//         glPushMatrix();

//         // write mouse population
//         string mouseStr = "mouse population: ";
//         mouseStr += to_string(mice.size());
        
//         unsigned char mouseCharPointer[mouseStr.size() + 1];
//         strcpy((char*) mouseCharPointer, mouseStr.c_str());

//         int len = strlen((char*) mouseCharPointer);

//         glColor3f(0.0, 0.0, 0.0);
//         glRasterPos2f(-0.9f, 0.79);
        
//         for (int i = 0; i < len; i++) {
//             glutBitmapCharacter(GLUT_BITMAP_8_BY_13, mouseCharPointer[i]);
//         }

//         // write cats population
//         string catStr = "cat population: ";
//         catStr += to_string(cats.size());
        
//         unsigned char catCharPointer[catStr.size() + 1];
//         strcpy((char*) catCharPointer, catStr.c_str());

//         len = strlen((char*) catCharPointer);

//         glColor3f(0.0, 0.0, 0.0);
//         glRasterPos2f(-0.9f, 0.73f);
        
//         for (int i = 0; i < len; i++) {
//             glutBitmapCharacter(GLUT_BITMAP_8_BY_13, catCharPointer[i]);
//         }

//         // write food amount
//         string foodStr = "food amount: ";
//         foodStr += to_string(foods.size());
        
//         unsigned char foodCharPointer[foodStr.size() + 1];
//         strcpy((char*) foodCharPointer, foodStr.c_str());

//         len = strlen((char*) foodCharPointer);

//         glColor3f(0.0, 0.0, 0.0);
//         glRasterPos2f(-0.9f, 0.67f);
        
//         for (int i = 0; i < len; i++) {
//             glutBitmapCharacter(GLUT_BITMAP_8_BY_13, foodCharPointer[i]);
//         }
        
//         glPopMatrix();

//}

// void drawEntities(void) {
//     glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT);

//     glPointSize(5.0f);
    

//     glBegin(GL_POINTS);
//         drawFoods();
//         drawCats();
//         drawMice();
//     glEnd();

//     writeRates();
//     writeStats();

//     glutSwapBuffers();
// }

// could go in GameManager
void initPop() {
    cats.clear();
    mice.clear();
    foods.clear();

    for(int i = 0; i < cat_population; i++){
        Cat* cat = new Cat(cat_smell_range, cat_speed, cat_reproduction_limiar);
        cat->childhood = 0;
        cat->pos.x = WIDTH-2;
        cat->pos.y = HEIGHT-2;
        cats.push_back(cat);
    }

    for(int i = 0; i < mouse_population; i++){
        Mouse* mouse = new Mouse(mouse_smell_range, mouse_speed, mouse_reproduction_limiar);
        mouse->childhood = 0;
        mouse->pos.x = (int)(WIDTH/2);
        mouse->pos.y = (int)(HEIGHT/2);
        mice.push_back(mouse);
    }

    for(int i = 0; i < food_amount; i++){
        Food* food = new Food(0, 0, 0);
        food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(food);
    }
}

// could go in GameManager
void makeStep() {

    score += (cats.size() >= mice.size() ? 1 : -1);

    for(int i = 0; i < mice.size(); i++){
        Mouse* m = mice[i];

        m->calculateReproductionUrge(1, 1);
        m->CheckRadar(cats, foods, mice);
        m->move(WIDTH, HEIGHT);

        // if is dead (energy = 0), delete
        if(m->energyConsume()){
            mice.erase(mice.begin() + i);
            i--;
        }
    }
    
    for(int i = 0; i < cats.size(); i++){
        Cat* c = cats[i];

        c->calculateReproductionUrge(1, 1);
        c->CheckRadar(mice, cats);
        c->move(WIDTH, HEIGHT);

        // if is dead (energy = 0), delete
        if(c->energyConsume()){
            cats.erase(cats.begin() + i);
            i--;
        }
    }


}

// could go in GameManager
void makeNewGeneration() {
    if(score == 0 ) ;// IDK 
    else if(score > 0) {
        // BUFF MICE
    } else {
        //BUFF CATS
    }
    initPop();
}

// could go in GameManager
void genFood() {
    if(!(rand() % food_spawn_difficulty)) {
        Food* food = new Food(0, 0, 0);
        food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(food);
    }
}

// void loop(int) {
//     genFood();

//     for (int i = 0; i < STEPS_PER_RENDER; i++)
//         makeStep();

//     glutPostRedisplay();

//     if (cats.size() == 0 || mice.size() == 0)
//         makeNewGeneration();

//     glutTimerFunc(1000/RENDERS_PER_SEC, loop, 0);
// }

// void runGame(int argc, char* argv[]) {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

//     glutInitWindowSize(640, 640);
//     glutCreateWindow("Tom&Jerry");

//     initPop();

//     glutDisplayFunc(drawEntities);
//     glutTimerFunc(0, loop, 0);

//     glutMainLoop();
// }

void runGame(int argc, char* argv[]) {
    for(int i = 0; i < 100000; i++)  {
        genFood();

        for (int i = 0; i < STEPS_PER_RENDER; i++)
            makeStep();

        if (cats.size() == 0 || mice.size() == 0)
            makeNewGeneration();

    }
    return;
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    runGame(argc, argv);

}  