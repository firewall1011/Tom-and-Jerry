#include "Mouse.h"
#include "Cat.h"
#include "Food.h"


void Mouse::CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food){
    // if isnt wandering, check if has eaten food
    if (current_state == RunningTo && pos.distance(tracked_pos) < 1) {
        if (food.size() > tracked_id && food[tracked_id].pos == tracked_pos){
            food.erase(food.begin() + tracked_id);
            energyRecover(1.0);
        }
        tracked_id = -1;
        tracked_pos = Vector2();
    }
    
    // change to Wandering, in case the food tracked is eaten, or ran away from cat
    current_state = Wandering;

    // look for the nearest cat to run away from, checking on smell_range
    double tracked_dist = smell_range;
    for(int i = 0; i < cats.size(); i++){
        Cat c = cats[i];
        if(pos.distance(c.pos) <= tracked_dist){
            current_state = RunningFrom;
            tracked_dist = pos.distance(c.pos);
            tracked_id = i;
            tracked_pos = c.pos;
        }
    }

    // check if could track a cat, and if is tracking, return 
    if (current_state == RunningFrom) return;

    // look for the nearest food to track, checking on smell_range
    // if can't track any food, keeps Wandering
    for(int i = 0; i < food.size(); i++){
        Food f = food[i];
        if(pos.distance(f.pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(f.pos);
            tracked_id = i;
            tracked_pos = f.pos;
        }
    }
}