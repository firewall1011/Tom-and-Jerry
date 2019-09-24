#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WIDTH 30
#define HEIGHT 30
#define EMPTY_SPACE 0
#define CAT 'c'
#define MOUSE 'm'
#define FOOD 'f'

enum state{
    Wandering, RunningFrom, RunningTo, Dead
};

class Vector2{
    public:
        int x, y;

    Vector2(){
        x = y = 0;
    }

    Vector2(int nx, int ny){
        x = nx;
        y = ny;
    }

    Vector2* sum(Vector2 v){
        Vector2* vn = new Vector2();
        vn->x = x + v.x;
        vn->y = y + v.y;
        return vn;
    }

    Vector2* sub(Vector2 v){
        Vector2* vn = new Vector2();
        vn->x = x - v.x;
        vn->y = y - v.y;
        return vn;
    }

    double distance(Vector2 v){
        double powx = pow(v.x - x, 2);
        double powy = pow(v.y - y, 2);
        double dist = sqrt( powx + powy);
        return dist;
    }

    double mag(){
        return sqrt(x*x + y*y);
    }


};

class Entity : public Vector2{
    public: 
        int smell_range;
        float speed;
        float hunger;
        float reproduction_urge;
        float atractiveness;
        state current_state;
        Vector2* pos;
        Vector2* tracked;

        Entity(){
            smell_range = 1;
            speed = 1;
            hunger = 0.5;
            reproduction_urge = 0;
            atractiveness = 0;
            pos = new Vector2();

            current_state = Wandering;
        }
        void move(){
            Vector2* v;
            if(current_state == Wandering){
                //Call wandering technique
                printf("Wandering\n");
                v = new Vector2((rand()%3 - 1) * speed, (rand()%3 - 1) * speed);
            }
            else if(current_state == RunningFrom){
                //Call runningFrom technique
                printf("Running From\n");
                v = pos->sub(*tracked);
                v->x = v->x / v->mag();
                v->y = v->y / v->mag();
                //Proprio menos alvo eh fugir
            }
            else if(current_state == RunningTo){
                //Call runningFrom technique
                printf("Running To\n");
                v = tracked->sub(*pos);
                v->x = v->x / v->mag();
                v->y = v->y / v->mag();
                //Proprio menos alvo eh fugir
            }
            v = pos->sum(*v);
            if(v->x >= 0 && v->y >= 0 && v->x < WIDTH && v->y < HEIGHT){
                    pos = v;
            }
        }
        void CheckRadar();
};

class Mouse : public Entity{
    public:
        Mouse(){
            smell_range = 20;
        }

        void CheckRadar(Entity* cat, Vector2* food){
            if(pos->distance(*cat->pos) <= smell_range){
                current_state = RunningFrom;
                tracked = cat->pos;
            }
            else if(pos->distance(*food) <= smell_range){
                current_state = RunningTo;
                tracked = food;
            }
            else{
                current_state = Wandering;
                tracked = NULL;
            }
        }
};

class Cat : public Entity{
    public:
        Cat(){
            smell_range = 15;
            speed = 2;
        }

        void CheckRadar(Mouse* mouse){
            if(pos->distance(*mouse->pos) <= smell_range){
                current_state = RunningTo;
                tracked = mouse->pos;
            }
            else{
                current_state = Wandering;
                tracked = NULL;
            }
        }
};


void PrintMap(char** grid, int w, int h){
    //system("clear");
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

    for(int i = 0; i < 20; i++)
        for(int j = 0; j < 20; j++)
            gridMap[i][j] = EMPTY_SPACE;

    srand(time(NULL));

    printf("here");
    Cat* cat = new Cat();
    Mouse* mouse = new Mouse();

    cat->pos->x = WIDTH-1;
    cat->pos->y = HEIGHT-1;

    Vector2* food = new Vector2(rand()%WIDTH, rand()%HEIGHT);

    gridMap[food->x][food->y] = FOOD;
    char c;
    //Game Loop
    //while((cat->pos->x != mouse->pos->x && cat->pos->y != mouse->pos->y) && (food.x != mouse->pos->x && food.y != mouse->pos->y)){
    while(true){
        gridMap[cat->pos->x][cat->pos->y] = EMPTY_SPACE;
        gridMap[mouse->pos->x][mouse->pos->y] = EMPTY_SPACE;
        cat->CheckRadar(mouse);
        mouse->CheckRadar(cat, food);
        cat->move();
        mouse->move();
        gridMap[mouse->pos->x][mouse->pos->y] = MOUSE;
        gridMap[cat->pos->x][cat->pos->y] = CAT;
        PrintMap(gridMap, WIDTH, HEIGHT);

        if(cat->pos->x == mouse->pos->x && cat->pos->y == mouse->pos->y){
            printf("CAT CAUGHT MOUSE!\n");
            break;
        } 
        if(food->x == mouse->pos->x && food->y == mouse->pos->y){
            printf("MOUSE CAUGHT FOOD!\n");
            break;
        } 
    }
    PrintMap(gridMap, WIDTH, HEIGHT);
}