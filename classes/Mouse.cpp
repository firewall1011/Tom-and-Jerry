#include "Mouse.h"
#include "Cat.h"
#include "Food.h"
#include "GameManager.h"


void Mouse::CheckRadar(std::vector<Cat>& cats, std::vector<Food>& food, std::vector<Mouse>& mice){
    // if isnt wandering, check if has got to partner
    if(current_state == RunningToPartner && pos.distance(tracked_pos) < 1){
        printf("Pop: %lu, (%lf,%lf) == (%lf,%lf)\n", mice.size(), mice[tracked_id].pos.x, mice[tracked_id].pos.y, tracked_pos.x, tracked_pos.y);
        if(reproduction_urge > 0.5 && mice.size() > tracked_id && mice[tracked_id].pos.distance(tracked_pos) <= 1){ // Guardar endereço de memoria do cara para ter uma referencia verdadeira, a posicao pode se alterar antes que chegue no proximo rato
            mice.push_back(GameManager::reproduct(*this, mice[tracked_id]));
            //addEnergy(-0.3);
            printf("\n\n----------------A new Mouse was borned--------------\n\n");
        }
        tracked_id = -1;
        tracked_pos = Vector2();
    }
    // if isnt wandering, check if has eaten food
    else if(current_state == RunningTo && pos.distance(tracked_pos) < 1) {
        if(food.size() > tracked_id && food[tracked_id].pos == tracked_pos){
            food.erase(food.begin() + tracked_id);
            addEnergy(1.0);
            printf("Mouse has eaten food\n");
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

    // if in need to reproduct, look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    if(reproduction_urge > 0.5){
        for(int i = 0; i < mice.size(); i++){
            Mouse e = mice[i];
            if(*this != e && pos.distance(e.pos) <= tracked_dist){
                current_state = RunningToPartner;
                tracked_dist = pos.distance(e.pos);
                tracked_id = i;
                tracked_pos = e.pos;
                printf("Mouse running to Partner with id %d\n", tracked_id);
            }
        }
    }

    // check if could track a mouse, and if is tracking, return
    if(current_state == RunningToPartner) return;

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