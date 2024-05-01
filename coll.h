#include "elem.h" 
#include "mov.h" 
#ifndef COLL_H
#define COLL_H
int collideWith(Ball* t, Paddle* o);
int collideWithTile(Ball* t, Tile* other); 
#endif 
