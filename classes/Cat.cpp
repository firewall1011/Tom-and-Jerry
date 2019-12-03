#include "Cat.h"
#include "GameManager.h"
#include <algorithm>

#define LIMIAR 0.6


//TODO: Remake state machine PROPERLY

void Cat::CheckRadar(std::vector<Mouse*>& mice, std::vector<Cat*>& cats){
    // if isnt wandering, check if has got to partner

    if(current_state == RunningToPartner && pos.distance(tracked->pos) < speed){
        Cat* partner = dynamic_cast<Cat*>(tracked);

        // std::cout << "tried to reproduce " << this << " " << partner->tracked << std::endl;
        if (reproduction_urge > LIMIAR && partner->current_state == RunningToPartner 
            && partner->reproduction_urge > LIMIAR && this == partner->tracked) {
        // i think this if is seg faulting, so need to change to dynamic_cast (here and mouse)
        // if(reproduction_urge > LIMIAR && cats.size() > tracked_id && cats[tracked_id].pos.distance(tracked_pos) <= 1 && cats[tracked_id].tracked_id != -1 && cats[tracked_id].current_state == RunningToPartner){
            cats.push_back(GameManager::reproduct(this, partner));
            addEnergy(-LIMIAR/2);
            // printf("\n\n----------------A new Cat was borned--------------\n\n");
        }
        // tracked_id = -1;
        // tracked_pos = Vector2();
        tracked = NULL;
    }
    // if isnt wandering, check if has eaten mouse
    else if(current_state == RunningTo && pos.distance(tracked->pos) < speed) {
        // if(mice.size() > tracked_id && mice[tracked_id].pos.distance(tracked_pos) < speed){
        // if (pos.distance(tracked->pos) < speed) {
        Mouse* dead = dynamic_cast<Mouse*>(tracked);
        // std::remove(mice.begin(), mice.end(), dead);
        if (mice.size() > tracked_id && mice[tracked_id] == tracked) {
            mice.erase(mice.begin() + tracked_id);
            addEnergy(1.0);
        // mice.erase(mice.begin() + tracked_id);
        }
        // printf("Cat has eaten Mouse\n");
        // }
        // tracked_id = -1;
        // tracked_pos = Vector2();
        tracked = NULL;
    }

    // change to Wandering to give a chance of mouse tracked run away
    current_state = Wandering;
    tracked = NULL;
    tracked_id = -1;

    // if in need to reproduct, look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    // std::cout << tracked << std::endl;
    double tracked_dist = smell_range;
    if(!childhood && reproduction_urge > LIMIAR){
        int id = -1;
        for(int i = 0; i < cats.size(); i++){
            Cat* c = cats[i];
            if (this == c)
                id = i;
            else if(pos.distance(c->pos) <= tracked_dist && c->reproduction_urge > LIMIAR){
                current_state = RunningToPartner;
                tracked_dist = pos.distance(c->pos);
                tracked_id = i;
                // tracked_pos = c.pos;
                tracked = c;
                
            }
        }
        // if (tracked != NULL)
        //     printf("Cat id %d running to Partner id %d\n", id, t_id);
    }

    // check if could track a cat, and if is tracking, return
    if(current_state == RunningToPartner) return;
    
    // look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    tracked_dist = smell_range;
    for(int i = 0; i < mice.size(); i++){
        Mouse* m = mice[i];
        if(pos.distance(m->pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(m->pos);
            tracked_id = i;
            // tracked_pos = m.pos;
            tracked = m;
        }
    }
}
