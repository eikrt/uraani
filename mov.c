#include "mov.h"
Vector reflect(Vector incident, Vector normal) {
    Vector reflection;
    float dot = dotProduct(incident, normal);
    reflection.x = -(incident.x - 2 * dot * normal.x);
    reflection.y = -(incident.y - 2 * dot * normal.y);
    return reflection;
}
void bounce(Ball *ballp, void* o) {
    Ball ball = *ballp;
    struct Paddle *other = (struct Paddle *)o;
    Vector normal = calcNormal(other);
    Vector r = reflect(ball.dir, normal);
    ballp->dir = r;
}
void bounceV(Ball *ballp, Vector normal) {
    Ball ball = *ballp;
    Vector r = reflect(ball.dir, normal);
    ballp->dir = r;
}
void moveBall(Ball* ball, Paddle* paddle) {

    int collide = collideWith(ball, paddle);
    if (collide == 0) {
    }
    if (collide == 1) {
        bounce(ball, paddle);
    }
    if (ball->p.x > 640 || ball->p.x < 0) {
            Vector normal = {1,0};
            bounceV(ball, normal);       
    }
    if (ball->p.y < 0) {
            Vector normal = {0,1};
            bounceV(ball, normal);       
    }
    float ang = atan2(ball->dir.y, ball->dir.x);
    ball->p.x += sin(ang) * ball->vel;
    ball->p.y += cos(ang) * ball->vel;
}
