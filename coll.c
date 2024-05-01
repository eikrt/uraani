#include "coll.h" 

int collideWith(Ball* this, Paddle* other) {

    if (this->p.x > other->p.x && this->p.x < other->p.x + other->size.x && this->p.y > other->p.y && this->p.y < other->p.y + other->size.y) {
        return 1;
    }
    return 0;
}
int collideWithTile(Ball* this, Tile* other) {
    if (this->p.x > other->p.x && this->p.x < other->p.x + other->size.x && this->p.y > other->p.y && this->p.y < other->p.y + other->size.y) {
        if (this->p.x - this->vel * this->dir.y < other->p.x) {
            Vector normal = {1,0};
            bounceV(this, normal);       
        }
        else if (this->p.x - this->vel * this->dir.y > other->p.x + other->size.x) {
            Vector normal = {1,0};
            bounceV(this, normal);       
        }
        else if (this->p.y - this->vel * this->dir.x < other->p.y) {
            Vector normal = {0,1};
            bounceV(this, normal);       
        }
        else if (this->p.y - this->vel * this->dir.x > other->p.y + other->size.y) {
            Vector normal = {0,1};
            bounceV(this, normal);       
        }
        other->alive = 0; 
        return 1;
    }
    return 0;
}
