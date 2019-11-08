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
#define INITIAL_MOUSE_POPULATION 20
#define INITIAL_CAT_POPULATION 5
#define MAX_FOOD 50

/* Rendering defines */
#define STEPS_PER_RENDER 30
#define RENDERS_PER_SEC 30

/* Declaration of arrays */
vector<Cat> cats;
vector<Mouse> mice;
vector<Food> foods;

void drawFoods() {
    for (Food f : foods)
        f.draw(WIDTH, HEIGHT);  
}

void drawCats() {
    for (Cat c : cats)
        c.draw(WIDTH, HEIGHT);
}

void drawMice() {
    for (Mouse m : mice)
        m.draw(WIDTH, HEIGHT);
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

    glutSwapBuffers();
}

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
    for(Mouse& m : mice){
        m.CheckRadar(cats, foods);
        m.move(WIDTH, HEIGHT);
        if(m.energyConsume()){
            std::remove(mice.begin(), mice.end(), m);
            //remover do array
        }
    }
    
    for(Cat& c : cats){
        c.CheckRadar(mice);
        c.move(WIDTH, HEIGHT);
        if(c.energyConsume()){
            std::remove(cats.begin(), cats.end(), c);
            //remover do array
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