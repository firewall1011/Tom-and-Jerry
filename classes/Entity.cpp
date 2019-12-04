#include "Entity.h"

#include <vector>
//#include <GL/glut.h>

#define DEBUG 0

Entity::Entity(int sr, float s, float rl) { 
    smell_range = sr;
    speed = s;
    reproduction_limiar = rl;
    reproduction_amount = 0; // fitness
    // rand initial energy, interval = [1.0, 0.5]
    energy = (((rand() % 100) / 100.0) + 1) / 2;
    reproduction_urge = 0.0;
    atractiveness = 0.0;
    current_state = Wandering;
    pos = Vector2();
    facing_dir = Vector2(rand()%3-1, rand()%3-1);
    representation = new (float [3]) {0.0, 0.0, 0.0};
    childhood = 100;
    tracked = NULL;
}

void Entity::move(int w, int h){
    Vector2 v;
    
    // Children cant reproduce, but each step, childhood decrease
    if (childhood) childhood -= 1;

    if (current_state == Wandering) {
        // Call wandering technique
        // Go ahead, go little up or go little down 
        if (facing_dir.mag() == 0) {
            // Only diagonals
            int dir_x = rand() % 2;
            int dir_y = rand() % 2;
            if (!dir_x) dir_x--;
            if (!dir_y) dir_y--;
            facing_dir.x = dir_x;
            facing_dir.y = dir_y;
        }

        double cosR = cos(M_PI / 4);
        double sinR = sin(M_PI / 4);

        int dir = (rand() % 3) - 1; // 1 little up, 0 ahead, -1 little down
        if (dir) {
            v = Vector2(facing_dir.x * cosR - facing_dir.y * sinR * dir, 
                        facing_dir.x * sinR * dir + facing_dir.y * cosR);
        } else {
            v = facing_dir;
        }
    } else if(current_state == RunningFrom) {
        // Call runningFrom technique
        v = (pos - tracked->pos);
    } else if(current_state == RunningTo || current_state == RunningToPartner) {
        // Call runningTo technique
        v = (tracked->pos - pos);
    }

    v.normalize();
    // To calculate validity, speed is important
    v *= speed;
    
    if (v.x + pos.x >= 0 && v.y + pos.y >= 0 && v.x + pos.x < w && v.y + pos.y < h) {
        //Valid position, face the new direction and move forward
        facing_dir = v / speed;
    } else {
        //Invalid position, turn to valid position and move forward
        if (pos.x + v.x >= w || pos.x + v.x < 0) {
            v.x = -v.x;
            if (pos.y + v.y >= h || pos.y + v.y < 0) v.y = -v.y;
            else v.y = 0;
        } else if (pos.y + v.y >= h || pos.y + v.y < 0) {
            v.y = -v.y;
            v.x = 0; 
        }

        v.normalize();
        facing_dir = v;
    }

    pos += facing_dir * speed;
}

bool Entity::energyConsume(){
    // changed from 100 to 1000, to make them last longer
    // could be K that is controlled by GA
    energy -= (speed*speed) / 1000;
    if(energy <= 0) return true;
        else return false;
}

void Entity::addEnergy(float amount){
    energy = energy+amount;
    if(energy > 1) energy = 1;
    else if(energy < 0) energy = 0;
}

/*
E = energy constant
N = population constant
*/
void Entity::calculateReproductionUrge(int E = 1, int N = 1){
    reproduction_urge = (energy*E)/(N);
}

// void Entity::draw(int w, int h) {
//     glColor3fv(getRepresentation());
//     glVertex2f(((((float) pos.x) / w) * 2) - 1.0f, 
//                 ((((float) pos.y) / h) * 2) - 1.0f);
// }