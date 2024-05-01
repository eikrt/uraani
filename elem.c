#include "elem.h" 

Vector subtractPoints(Point p1, Point p2) {
    Vector result;
    result.x = p1.x - p2.x;
    result.y = p1.y - p2.y;
    return result;
}

Vector crossProduct(Vector v1, Vector v2) {
    Vector result;
    result.x = v1.y * v2.y;
    result.y = v2.x - v1.x;
    return result;
}
float dotProduct(Vector v1, Vector v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
float magnitude(Vector v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector normalize(Vector v) {
    float mag = magnitude(v);
    v.x /= mag;
    v.y /= mag;
    return v;
}
Vector getNormalVec(Point p1, Point p2) {
    Vector vec = subtractPoints(p2, p1);
    Vector normal = {vec.y, vec.x}; 
    normal = normalize(normal);
    return normal;
}
Vector calcNormal(Paddle* other) {
    Point secondPoint = {other->p.x + other->size.x, other->p.y};
    Vector normal = getNormalVec(other->p, secondPoint);
    return normal;
}
int randI() {
    //    int random_number;
      //  srand(time(NULL));
   //     random_number = rand();
   //     return random_number;
   return 0;
}
