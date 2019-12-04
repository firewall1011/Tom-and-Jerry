#include "Entity.h"
#include "Cat.h"
#include "Mouse.h"
#include "GameManager.h"

namespace GameManager {
    int mutation_rate = 10;
    float speed_mutation = 1.0f;
    float smell_range_mutation = 0.5f;
    
    Cat* reproduct(Cat* c1, Cat* c2){
        Cat* child = new Cat();

        child->smell_range = (c1->smell_range + c2->smell_range)/2;
        child->speed = (c1->speed + c2->speed)/2;
        child->pos = c1->pos;

        mutate(child);

        return child;
    }

    Mouse* reproduct(Mouse* m1, Mouse* m2){
        Mouse* child = new Mouse();

        child->smell_range = (m1->smell_range + m2->smell_range)/2;
        child->speed = (m1->speed + m2->speed)/2;
        child->pos = m1->pos;

        mutate(child);

        return child;
    }

    void mutate(Cat* cat){
        cat->speed +=   (!(rand() % mutation_rate)) ? 
                            ((rand() % 2) && cat->speed > speed_mutation ? 
                                -speed_mutation 
                                : speed_mutation) 
                            : 0;
        cat->smell_range +=     (!(rand() % mutation_rate)) ? 
                                    ((rand() % 2) && cat->smell_range > smell_range_mutation ? 
                                        -smell_range_mutation  
                                        : smell_range_mutation) 
                                    : 0;
    }

    void mutate(Mouse* mouse){
        mouse->speed +=     (!(rand() % mutation_rate)) ? 
                                ((rand() % 2) && mouse->speed > speed_mutation ? 
                                    -speed_mutation  
                                    : speed_mutation) 
                                : 0;
        mouse->smell_range +=   (!(rand() % mutation_rate)) ? 
                                    ((rand() % 2) && mouse->smell_range > smell_range_mutation ? 
                                        -smell_range_mutation  
                                        : smell_range_mutation) 
                                    : 0;
    }
}