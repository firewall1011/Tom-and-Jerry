#include "Cat.h"
#include "GameManager.h"

void Cat::CheckRadar(std::vector<Mouse>& mice, std::vector<Cat>& cats){
    // if isnt wandering, check if has got to partner
    if(current_state == RunningToPartner && pos.distance(tracked_pos) < 1){
        if(reproduction_urge > 0.5 && cats.size() > tracked_id && cats[tracked_id].pos == tracked_pos){
            cats.push_back(GameManager().reproduct(*this, cats[tracked_id]));
            addEnergy(-0.3);
        }
        tracked_id = -1;
        tracked_pos = Vector2();
    }
    // if isnt wandering, check if has eaten mouse
    else if(current_state == RunningTo && pos.distance(tracked_pos) < 1) {
        if(mice.size() > tracked_id && mice[tracked_id].pos.distance(tracked_pos) < 1){
            mice.erase(mice.begin() + tracked_id);
            addEnergy(1.0);
        }
        tracked_id = -1;
        tracked_pos = Vector2();
    }

    // change to Wandering to give a chance of mouse tracked run away
    current_state = Wandering;


    // if in need to reproduct, look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    double tracked_dist = smell_range;
    if(reproduction_urge > 0.5){
        for(int i = 0; i < cats.size(); i++){
            Entity e = cats[i];
            if(pos.distance(e.pos) <= tracked_dist){
                current_state = RunningToPartner;
                tracked_dist = pos.distance(e.pos);
                tracked_id = i;
                tracked_pos = e.pos;
            }
        }
    }

    // check if could track a cat, and if is tracking, return
    if(current_state == RunningToPartner) return;
    
    // look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    for(int i = 0; i < mice.size(); i++){
        Mouse m = mice[i];
        if(pos.distance(m.pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(m.pos);
            tracked_id = i;
            tracked_pos = m.pos;
        }
    }
}
