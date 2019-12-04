#include "Cat.h"
#include "GameManager.h"
#include <algorithm>


void Cat::CheckRadar(std::vector<Mouse*>& mice, std::vector<Cat*>& cats){
    // if isnt wandering, check if has got to partner
    if(current_state == RunningToPartner && pos.distance(tracked->pos) < speed){
        
        Cat* partner = dynamic_cast<Cat*>(tracked);
        
        if (reproduction_urge > reproduction_limiar && partner->current_state == RunningToPartner 
                && partner->reproduction_urge > reproduction_limiar && this == partner->tracked) {
            cats.push_back(GameManager::reproduct(this, partner));
            addEnergy(-reproduction_limiar/2);
        }

        tracked = NULL;
    }

    // if isnt wandering, check if has eaten mouse
    else if(current_state == RunningTo && pos.distance(tracked->pos) < speed) {
        
        Mouse* dead = dynamic_cast<Mouse*>(tracked);
        
        if (mice.size() > tracked_id && mice[tracked_id] == tracked) {
            mice.erase(mice.begin() + tracked_id);
            addEnergy(1.0);
        }
        
        tracked = NULL;
    }

    // change to Wandering to give a chance of mouse tracked run away
    current_state = Wandering;
    tracked = NULL;
    tracked_id = -1;

    // if in need to reproduct, look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    double tracked_dist = smell_range;
    if(!childhood && reproduction_urge > reproduction_limiar){
        int id = -1;
        for(int i = 0; i < cats.size(); i++){
            Cat* c = cats[i];
            if (this == c)
                id = i;
            else if(pos.distance(c->pos) <= tracked_dist && c->reproduction_urge > reproduction_limiar){
                current_state = RunningToPartner;
                tracked_dist = pos.distance(c->pos);
                tracked_id = i;
                tracked = c;
                
            }
        }
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
            tracked = m;
        }
    }
}
