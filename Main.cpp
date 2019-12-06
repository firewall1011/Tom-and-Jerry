#include <vector>
#include <cmath>
#include <chrono>
#include <ctime>
#include <GL/glut.h>
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

/* Genetic Algorithm defines */
#define INITIAL_MOUSE_POPULATION 1000
#define INITIAL_CAT_POPULATION 2
#define INITIAL_FOOD_AMOUNT 50
#define FOOD_SPAWN_RATE 30

/* Rendering defines */
#define STEPS_PER_RENDER 1
#define RENDERS_PER_SEC 30



/* Genetic Algorithm declarations */
int score = 0;
chrono::system_clock::time_point genStart;
int genNum = 1;

// will save the best entity params
struct best {   
    int suv_time = -1;
    int sml_rng;
    float spd;
    float rep_lim;
};

struct best best_cat;
struct best best_mouse;

//Cats params
int cat_population = 10;
int cat_smell_range = 5;
float cat_speed = 1;
float cat_reproduction_limiar = 0.6;


//Mice params
int mouse_population = 50;
int mouse_smell_range = 5;
float mouse_speed = 1;
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

void writeRates() {
    glPushMatrix();

    string ratestr = "running on ";
    ratestr += to_string(STEPS_PER_RENDER * RENDERS_PER_SEC);
    ratestr += " steps/sec";

    unsigned char rate[ratestr.size() + 1];
    strcpy((char*) rate, ratestr.c_str());

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-0.9, 0.9);

    int len = strlen((char*) rate);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, rate[i]);
    }

    glPopMatrix();
}

void writeStats() {
        glPushMatrix();

        // write mouse population
        string mouseStr = "mouse population: ";
        mouseStr += to_string(mice.size());
        
        unsigned char mouseCharPointer[mouseStr.size() + 1];
        strcpy((char*) mouseCharPointer, mouseStr.c_str());

        int len = strlen((char*) mouseCharPointer);

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(-0.9f, 0.79);
        
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, mouseCharPointer[i]);
        }

        // write cats population
        string catStr = "cat population: ";
        catStr += to_string(cats.size());
        
        unsigned char catCharPointer[catStr.size() + 1];
        strcpy((char*) catCharPointer, catStr.c_str());

        len = strlen((char*) catCharPointer);

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(-0.9f, 0.73f);
        
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, catCharPointer[i]);
        }

        // write food amount
        string foodStr = "food amount: ";
        foodStr += to_string(foods.size());
        
        unsigned char foodCharPointer[foodStr.size() + 1];
        strcpy((char*) foodCharPointer, foodStr.c_str());

        len = strlen((char*) foodCharPointer);

        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(-0.9f, 0.67f);
        
        for (int i = 0; i < len; i++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, foodCharPointer[i]);
        }
        
        glPopMatrix();

}

void drawEntities(void) {
    glClearColor(0.5f, 0.8f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5.0f);
    

    glBegin(GL_POINTS);
        drawFoods();
        drawCats();
        drawMice();
    glEnd();

    writeRates();
    writeStats();

    glutSwapBuffers();
}

// could go in GameManager
void initPop() {
    cats.clear();
    mice.clear();
    foods.clear();

    for(int i = 0; i < INITIAL_CAT_POPULATION; i++){
        Cat* cat = new Cat();
        cat->childhood = 0;
        cat->pos.x = WIDTH-2;
        cat->pos.y = HEIGHT-2;
        cats.push_back(cat);
    }

    for(int i = 0; i < INITIAL_MOUSE_POPULATION; i++){
        Mouse* mouse = new Mouse();
        mouse->childhood = 0;
        mouse->pos.x = (int)(WIDTH/2);
        mouse->pos.y = (int)(HEIGHT/2);
        mice.push_back(mouse);
    }

    for(int i = 0; i < INITIAL_FOOD_AMOUNT; i++){
        Food* food = new Food();
        food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(food);
    }
    genStart = chrono::system_clock::now();
    time_t genStartTime = chrono::system_clock::to_time_t(genStart);
    cout << "Generation " << genNum << " started with " << cat_population << " cats and " << mouse_population << " mice." << endl;
    cout << "Cat stats: " << endl;
    cout << "\tsmrange: " << cat_smell_range << endl;
    cout << "\tspeed:   " << cat_speed << endl;
    cout << "\treprlim: " << cat_reproduction_limiar << endl;
    cout << "Mouse stats: " << endl;
    cout << "\tsmrange: " << mouse_smell_range << endl;
    cout << "\tspeed:   " << mouse_speed << endl;
    cout << "\treprlim: " << mouse_reproduction_limiar << endl;
}

// could go in GameManager
void makeStep() {

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
void makeNewGeneration(bool catWon) {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    chrono::duration<double> genDuration = now - genStart;
    cout << "Generation " << genNum++ << " survived for " << genDuration.count() << " seconds." << endl;
    if(catWon) {
        cout << "Cats Won" << endl;
        //BUFF MICE
        mouse_smell_range = (mouse_smell_range + best_mouse.sml_rng) / 2;
        mouse_reproduction_limiar = (mouse_reproduction_limiar + best_mouse.rep_lim) / 2;
        mouse_speed = (mouse_speed + best_mouse.spd) / 2;

        //NERF CATS
        if(rand() % 2) cat_smell_range = cat_smell_range * 0.9; /*(0.9 + ((rand() % 2) / 10)) ;*/
        if(cat_smell_range == 0) cat_smell_range = 1;
        if(rand() % 2) cat_reproduction_limiar = cat_reproduction_limiar * 1.1; /*(0.9 + ((rand() % 2) / 10)) ;*/
        if(cat_reproduction_limiar > 1) cat_reproduction_limiar = 0.9;
        if(rand() % 2) cat_speed = cat_speed * 0.9; /*(0.9 + ((rand() % 2) / 10)) ;*/
        if(cat_speed == 0) cat_speed = 0.1;
    } else {
        cout << "Mice Won" << endl;
        // BUFF CATS
        cat_smell_range = (cat_smell_range + best_cat.sml_rng) / 2;
        cat_reproduction_limiar = (cat_reproduction_limiar + best_cat.rep_lim) / 2;
        cat_speed = (cat_speed + best_cat.spd) / 2;
        
        //NERF MICE
        if(rand() % 2) mouse_smell_range = mouse_smell_range * 0.9; /*(0.9 + ((rand() % 2) / 10)) ;*/
        if(mouse_smell_range == 0) mouse_smell_range = 1;
        if(rand() % 2) mouse_reproduction_limiar = mouse_reproduction_limiar * 1.1; /*(0.9 + ((rand() % 2) / 10)) ;*/
        if(mouse_reproduction_limiar > 1) mouse_reproduction_limiar = 0.9;
        if(rand() % 2) mouse_speed = mouse_speed * 0.9; /*(0.9 + ((rand() % 2) / 10)) ;*/
        if(mouse_speed == 0) mouse_speed = 0.1;
    }
    best_mouse.suv_time = best_cat.suv_time = -1;
    cout << endl << endl;
    score = 0;
    initPop();
}

// could go in GameManager
void genFood() {
    if(!(rand() % FOOD_SPAWN_RATE)) {
        Food* food = new Food();
        food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(food);
    }
}

void loop(int) {
    genFood();

    for (int i = 0; i < STEPS_PER_RENDER; i++)
        makeStep();

    glutPostRedisplay();

    if (!cats.size())
        makeNewGeneration(false);

    if(!mice.size())
        makeNewGeneration(true);

    glutTimerFunc(1000/RENDERS_PER_SEC, loop, 0);
}

void runGame(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(640, 640);
    glutCreateWindow("Tom&Jerry");

    initPop();

    glutDisplayFunc(drawEntities);
    glutTimerFunc(0, loop, 0);

    glutMainLoop();
}

// void runGame(int argc, char* argv[]) {
//     initPop();

//     while(genNum <= 1000)  {
//         genFood();

//         for (int i = 0; i < STEPS_PER_RENDER; i++)
//             makeStep();

        // if (!cats.size())
        //     makeNewGeneration(false);

        // if(!mice.size())
        //     makeNewGeneration(true);

//     }
//     return;
// }

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    runGame(argc, argv);

}  