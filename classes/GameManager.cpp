#include "Entity.h"
#include "Cat.h"
#include "Mouse.h"
#include "GameManager.h"

namespace GameManager{
    float mutation_rate = 0.1f;
    float speed_mutation = 1.0f;
    float smell_range_mutation = 0.5f;
    Cat* reproduct(Cat* c1, Cat* c2){
        Cat* child = new Cat();
        child->smell_range = (c1->smell_range + c2->smell_range)/2;
        child->speed = (c1->speed + c2->speed)/2;
        child->pos = c1->pos;
        mutate(child);
        // printChild(child);
        return child;
    }
    Mouse* reproduct(Mouse* m1, Mouse* m2){
        Mouse* child = new Mouse();
        child->smell_range = (m1->smell_range + m2->smell_range)/2;
        child->speed = (m1->speed + m2->speed)/2;
        child->pos = m1->pos;
        mutate(child);
        // printChild(child);
        return child;
    }
    void mutate(Cat* c){
        int m = 1/mutation_rate;
        c->speed += (!(rand() % m)) ? (rand() % 2 ? speed_mutation  : -speed_mutation) : 0;
        c->speed += (!(rand() % m)) ? (rand() % 2 ? smell_range_mutation  : -smell_range_mutation) : 0;
    }
    void mutate(Mouse* mouse){
        int m = 1/mutation_rate;
        mouse->speed += (!(rand() % m)) ? (rand() % 2 ? speed_mutation  : -speed_mutation) : 0;
        mouse->speed += (!(rand() % m)) ? (rand() % 2 ? smell_range_mutation  : -smell_range_mutation) : 0;
    }
    void printChild(Mouse& e){
        printf("(%lf,%lf) | sm: %d | sp: %f\n", e.pos.x, e.pos.y, e.smell_range, e.speed); 
    }
    void printChild(Cat& e){
        printf("(%lf,%lf) | sm: %d | sp: %f\n", e.pos.x, e.pos.y, e.smell_range, e.speed); 
    }
}