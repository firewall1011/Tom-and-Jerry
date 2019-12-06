#include "Entity.h"
#include <GL/glut.h>


void Entity::draw(int w, int h) {
    glColor3fv(getRepresentation());
    glVertex2f(((((float) pos.x) / w) * 2) - 1.0f, 
                ((((float) pos.y) / h) * 2) - 1.0f);
}