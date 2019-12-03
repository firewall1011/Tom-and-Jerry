#ifndef MOUSE_H
#define MOUSE_H

#include "Entity.h"
#include "Cat.h"
#include "Food.h"

#include <vector>

class Cat;
class Food;

class Mouse : public Entity{
public:
    Mouse(){
        setRepresentation(new (float [3]) {90.0f/255, 50.0f/255, 26.0f/255});
    }
    void CheckRadar(std::vector<Cat*>& cats, std::vector<Food*>& food, std::vector<Mouse*>& mice);
};


#endif