#ifndef ELEM_H
#define ELEM_H
#include <time.h>
#include <math.h>
#include "elem.h"
typedef enum {
    Def 
} TileType; 

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float x;
    float y;
} Vector;
typedef struct {
    Point p;
    Point size;
    TileType ttype;
    int alive;
} Tile;
typedef struct Ball {
        Point p;
        Point size;
        float vel;
        Vector dir;
} Ball;

typedef struct Paddle {
        Point p;
        Point size;
        float vel;
        float dir;
} Paddle;

Vector subtractPoints(Point p1, Point p2);
Vector crossProduct(Vector v1, Vector v2);
float dotProduct(Vector v1, Vector v2);
float magnitude(Vector v); 
Vector normalize(Vector v);
Vector getNormalVec(Point p1, Point p2);
Vector calcNormal(Paddle* other);
int randI();
#endif 
