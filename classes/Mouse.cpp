#include "Mouse.h"
#include "Cat.h"
#include "Food.h"
#include "GameManager.h"
#include <algorithm>

#define LIMIAR 0.6

void Mouse::CheckRadar(std::vector<Cat*>& cats, std::vector<Food*>& food, std::vector<Mouse*>& mice){
    // if isnt wandering, check if has got to partner
    
    if(current_state == RunningToPartner && pos.distance(tracked->pos) < speed){
        // printf("Pop: %lu, (%lf,%lf) == (%lf,%lf)\n", mice.size(), mice[tracked_id].pos.x, mice[tracked_id].pos.y, tracked_pos.x, tracked_pos.y);
        Mouse* partner = dynamic_cast<Mouse*>(tracked);
        // correct with pointer and dynamic_cast
        if (reproduction_urge > LIMIAR && partner->current_state == RunningToPartner 
            && partner->reproduction_urge > LIMIAR && partner->tracked == this) {
        // if(reproduction_urge > LIMIAR && mice.size() > tracked_id && mice[tracked_id].pos.distance(tracked_pos) <= mice[tracked_id].speed && mice[tracked_id].tracked_id != -1 && mice[tracked_id].current_state == RunningToPartner){ // Guardar endereço de memoria do cara para ter uma referencia verdadeira, a posicao pode se alterar antes que chegue no proximo rato
            mice.push_back(GameManager::reproduct(this, partner));
            addEnergy(-(LIMIAR / 2.0));
            // printf("\n\n----------------A new Mouse was borned--------------\n\n");
        }
        // tracked_id = -1;
        // tracked_pos = Vector2();
        tracked = NULL;
    }
    // if isnt wandering, check if has eaten food
    else if(current_state == RunningTo && pos.distance(tracked->pos) < speed) {
        // if(food.size() > tracked_id && food[tracked_id].pos == tracked_pos){
        Food* eaten = dynamic_cast<Food*>(tracked);
        // std::cout << "bef: " << food.size() << std::endl;
        // std::remove(food.begin(), food.end(), eaten);
        if (food.size() > tracked_id && food[tracked_id] == tracked) {
            food.erase(food.begin() + tracked_id);
            addEnergy(1.0);
        }
        // std::cout << "aft: " << food.size() << std::endl;
        // food.erase(food.begin() + tracked_id);
        // printf("Mouse has eaten food\n");
        // }
        // tracked_id = -1;
        // tracked_pos = Vector2();
        tracked = NULL;
    }
    
    // change to Wandering, in case the food tracked is eaten, or ran away from cat
    current_state = Wandering;
    tracked = NULL;
    tracked_id = -1;

    // look for the nearest cat to run away from, checking on smell_range
    double tracked_dist = smell_range;
    for(int i = 0; i < cats.size(); i++){
        Cat* c = cats[i];
        if(pos.distance(c->pos) <= tracked_dist){
            current_state = RunningFrom;
            tracked_dist = pos.distance(c->pos);
            tracked_id = i;
            // tracked_pos = c.pos;
            tracked = c;
        }
    }

    // check if could track a cat, and if is tracking, return 
    if (current_state == RunningFrom) return;

    // if in need to reproduct, look for the nearest mouse to track, checking on smell_range
    // if can't track any mouse, keeps Wandering
    if(!childhood && reproduction_urge > LIMIAR){
        int id = -1;
        for(int i = 0; i < mice.size(); i++){
            Mouse* m = mice[i];
            if (this == m)
                id = i;
            else if (pos.distance(m->pos) <= tracked_dist && m->reproduction_urge > LIMIAR) {
                current_state = RunningToPartner;
                tracked_dist = pos.distance(m->pos);
                tracked_id = i;
                // tracked_pos = m.pos;
                tracked = m;
            }
        }
        // if (tracked != NULL)
        //     printf("Mouse id %d running to Partner with id %d\n",id, t_id);
    }

    // check if could track a mouse, and if is tracking, return
    if(current_state == RunningToPartner) return;

    // look for the nearest food to track, checking on smell_range
    // if can't track any food, keeps Wandering
    for(int i = 0; i < food.size(); i++){
        Food* f = food[i];
        if(pos.distance(f->pos) <= tracked_dist){
            current_state = RunningTo;
            tracked_dist = pos.distance(f->pos);
            tracked_id = i;
            // tracked_pos = f.pos;
            tracked = f;
        }
    }
}