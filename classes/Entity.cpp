#include "Entity.h"

#include <vector>
#include <GL/glut.h>

#define DEBUG 0

Entity::Entity() { 
    smell_range = 5;
    speed = 1;
    hunger = 0.0;
    reproduction_urge = 0.0;
    atractiveness = 0.0;
    current_state = Wandering;
    pos = Vector2();
    tracked_id = -1;
    tracked_pos = Vector2();
    representation = new (float [3]) {0.0, 0.0, 0.0};
}

void Entity::move(int w, int h){
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
        //Call runningTo technique
        v = (tracked_pos - pos).normal();
    }

    v.roundInt();

    if( v.x + pos.x >= 0 && v.y + pos.y >= 0 && v.x + pos.x < w && v.y + pos.y < h){
        v += pos;
    }
    else{
        // explain this please! this is bugging me (/alex)
        v -= pos;
    }
    
    if(v.x >= 0 && v.y >= 0 && v.x < w && v.y < h) 
        pos = v;
}

bool Entity::energyConsume(){
    energy -= (speed*speed)/100;
    if(energy <= 0) return true;
        else return false;
}

void Entity::energyRecover(float amount){
    energy = (energy+amount < 1) ? energy+amount : 1;
}

void Entity::draw(int w, int h) {
    glColor3fv(getRepresentation());
    glVertex2f(((((float) pos.x) / w) * 2) - 1.0f, 
                ((((float) pos.y) / h) * 2) - 1.0f);
}