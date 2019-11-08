#include "Cat.h"

void Cat::CheckRadar(std::vector<Mouse>& mice){
    // if isnt wandering, check if has eaten mouse
    if (current_state == RunningTo && pos.distance(tracked_pos) < 1) {
        if (mice.size() > tracked_id && mice[tracked_id].pos.distance(tracked_pos) < 1){
            mice.erase(mice.begin() + tracked_id);
            energyRecover(1);
        }
        tracked_id = -1;
        tracked_pos = Vector2();
    }

    // change to Wandering to give a chance of mouse tracked run away
    current_state = Wandering;

    // look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    double tracked_dist = smell_range;
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
