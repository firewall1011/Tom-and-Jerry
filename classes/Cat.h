#ifndef CAT_H
#define CAT_H

#include "Entity.h"
#include "Mouse.h"

#include <vector>

class Mouse;

class Cat : public Entity{
public:
    Cat(float sr, float s, float rl): Entity(sr, s, rl){
        setRepresentation(new (float [3]) {0.4f, 0.4f, 0.4f});
    }
    void CheckRadar(std::vector<Mouse*>& mice, std::vector<Cat*>& cats);
};

#endif