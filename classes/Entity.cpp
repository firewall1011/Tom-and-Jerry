#include "Entity.h"

#include <vector>
#include <GL/glut.h>

#define DEBUG 0

Entity::Entity() { 
    smell_range = 5;
    speed = 1;
    // rand initial energy, interval = [1.0, 0.5]
    energy = (((rand() % 100) / 100.0) + 1) / 2;
    reproduction_urge = 0.0;
    atractiveness = 0.0;
    current_state = Wandering;
    tracked_id = -1;
    pos = Vector2();
    facing_dir = Vector2();
    tracked_pos = Vector2();
    representation = new (float [3]) {0.0, 0.0, 0.0};
}

void Entity::move(int w, int h){
    Vector2 v;
    if(current_state == Wandering){
        //Call wandering technique
        //v = Vector2((rand()%3 - 1), (rand()%3 - 1)) * speed;
        if(facing_dir.x == 0) v = facing_dir + Vector2(rand()%3-1, 0);
        else if(facing_dir.y == 0) v = facing_dir + Vector2(0, rand()%3-1);
        else{
            int q = rand()%2; int dir = rand()%2;
            if(!dir){
                if(facing_dir.x == 1) v = facing_dir + Vector2(-q, 0);
                else v = facing_dir + Vector2(q, 0);
            }
            else{
                if(facing_dir.y == 1) v = facing_dir + Vector2(0, -q);
                else v = facing_dir + Vector2(0, q);
            }
        }
    }
    else if(current_state == RunningFrom){
        //Call runningFrom technique
        v = (pos - tracked_pos).normal();
    }
    else if(current_state == RunningTo){
        //Call runningTo technique
        v = (tracked_pos - pos).normal();
    }

    v.roundInt();

    if( v.x + pos.x >= 0 && v.y + pos.y >= 0 && v.x + pos.x < w && v.y + pos.y < h){
        //Valid position, face the new direction and move forward
        facing_dir = v;
        pos += facing_dir;
    }
    else{
        //Invalid position, turn to valid position and move forward
        if(pos.x + v.x < 0 || pos.x + v.x >= w){
            v.x = 0;
            if(pos.y + v.y < 0 || pos.y + v.y >= h) v.y = -v.y;
            else if(v.y == 0){
                if(pos.y + 1 < h) v.y = 1; //Da preferencia para ir pra cima
                else v.y = -1;
            }
        }
        else if((pos.y + v.y < 0 || pos.y + v.y >= h){
            v.y = 0;
            if(pos.x + v.x < 0 || pos.x + v.x >= w) v.x = -v.x;
            else if(v.x == 0){  //Da preferencia para ir pra direita
                if(pos.x + 1 < w) v.x = 1;
                else v.x = -1;
            }
        }
        facing_dir = v;
        pos += v;
    }
}

bool Entity::energyConsume(){
    // changed from 100 to 1000, to make them last longer
    energy -= (speed*speed) / 1000;
    if(energy <= 0) return true;
        else return false;
}

void Entity::energyRecover(float amount){
    energy = (energy + amount < 1) ? energy + amount : 1;
}

void Entity::draw(int w, int h) {
    glColor3fv(getRepresentation());
    glVertex2f(((((float) pos.x) / w) * 2) - 1.0f, 
                ((((float) pos.y) / h) * 2) - 1.0f);
}