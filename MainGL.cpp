#include <GL/glut.h>
#include <string>
#include <cstring>

#include "classes/GameManager.h"


using namespace GameManager;

/* Rendering defines */
#define STEPS_PER_RENDER 100
#define RENDERS_PER_SEC 30


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

    std::string ratestr = "running on ";
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
        std::string mouseStr = "mouse population: ";
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
        std::string catStr = "cat population: ";
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
        std::string foodStr = "food amount: ";
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

int main(int argc, char* argv[]){
    srand(RAND_SEED);
    
    runGame(argc, argv);
}  