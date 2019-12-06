#ifndef FOOD_H
#define FOOD_H

#include "Entity.h"

class Food;

class Food : public Entity{
public:
    Food(float sr, int s, float rl) : Entity(sr, s, rl) {
        setRepresentation(new (float [3]) {1.0f, 222.0f/255, 4.0f/255});
    }
};

#endif