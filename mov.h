#ifndef MOV_H
#define MOV_H
#include "elem.h"
#include "coll.h"
#include <stdio.h>
Vector reflect(Vector incident, Vector normal);
void bounce(Ball *ballp, void* o);
void bounceV(Ball *ballp, Vector normal);
void moveBall(Ball* ball, Paddle* paddle);
#endif
