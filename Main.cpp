#include <vector>
#include <cmath>
#include <GL/glut.h>
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
#define INITIAL_MOUSE_POPULATION 10
#define INITIAL_CAT_POPULATION 3
#define MAX_FOOD 50

/* Rendering defines */
#define STEPS_PER_RENDER 1
#define RENDERS_PER_SEC 30

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

void initPop() {
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

    for(int i = 0; i < MAX_FOOD; i++){
        Food* food = new Food();
        food->pos = Vector2(rand()%WIDTH, rand()%HEIGHT);
        foods.push_back(food);
    }
}

//BUG QUANDO COME

void makeStep() {
    for(int i = 0; i < mice.size(); i++){
        Mouse* m = mice[i];

        m->calculateReproductionUrge(1, 1, 1);
        m->CheckRadar(cats, foods, mice);
        m->move(WIDTH, HEIGHT);

        // if is dead (energy = 0), delete
        if(m->energyConsume()){
            mice.erase(mice.begin() + i);
            i--;
            // printf("Mouse died hunger\nPopulation: %lui\n", mice.size());            
        }
    }
    
    for(int i = 0; i < cats.size(); i++){
        Cat* c = cats[i];

        c->calculateReproductionUrge(1, 1, 1);
        // c.reproduction_urge = 0.0f;
        c->CheckRadar(mice, cats);
        c->move(WIDTH, HEIGHT);

        // if is dead (energy = 0), delete
        if(c->energyConsume()){
            cats.erase(cats.begin() + i);
            i--;
            // printf("Cat died hunger\nPopulation: %lui\n", cats.size());  
        }
    }
}

void loop(int) {
    for (int i = 0; i < STEPS_PER_RENDER; i++)
        makeStep();

    glutPostRedisplay();

    glutTimerFunc(1000/RENDERS_PER_SEC, loop, 0);
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(640, 640);
    glutCreateWindow("Tom&Jerry");

    initPop();

    glutDisplayFunc(drawEntities);
    glutTimerFunc(0, loop, 0);

    glutMainLoop();
}  