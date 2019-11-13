#include "Entity.h"
#include "Cat.h"
#include "Mouse.h"
#include "GameManager.h"

namespace GameManager{
    float mutation_rate = 0.1f;
    float speed_mutation = 1.0f;
    float smell_range_mutation = 0.5f;
    Cat reproduct(Cat& p1, Cat& p2){
        Cat child = Cat();
        child.smell_range = (p1.smell_range + p2.smell_range)/2;
        child.speed = (p1.speed + p2.speed)/2;
        child.pos = p1.pos;
        mutate(child);
        printChild(child);
        return child;
    }
    Mouse reproduct(Mouse& p1, Mouse& p2){
        Mouse child = Mouse();
        child.smell_range = (p1.smell_range + p2.smell_range)/2;
        child.speed = (p1.speed + p2.speed)/2;
        child.pos = p1.pos;
        mutate(child);
        printChild(child);
        return child;
    }
    void mutate(Cat& c){
        int m = 1/mutation_rate;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? speed_mutation  : -speed_mutation) : 0;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? smell_range_mutation  : -smell_range_mutation) : 0;
    }
    void mutate(Mouse& c){
        int m = 1/mutation_rate;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? speed_mutation  : -speed_mutation) : 0;
        c.speed += (!(rand() % m)) ? (rand() % 2 ? smell_range_mutation  : -smell_range_mutation) : 0;
    }
    void printChild(Mouse& e){
        printf("(%lf,%lf) | sm: %d | sp: %f\n", e.pos.x, e.pos.y, e.smell_range, e.speed); 
    }
    void printChild(Cat& e){
        printf("(%lf,%lf) | sm: %d | sp: %f\n", e.pos.x, e.pos.y, e.smell_range, e.speed); 
    }
}