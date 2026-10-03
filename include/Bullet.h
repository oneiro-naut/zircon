#ifndef BULLET_H
#define BULLET_H
#include "Object.h"

// class Object;
class Game;

class Bullet : public Object
{
public:
    Bullet(obj_t btype, float x, float y, float vx, float vy, Texture sprt);
    virtual void update();
    ~Bullet();
    virtual void hasCollided(obj_t withtype, ZirconRect overlap_r);
    virtual void checkBoundaryCollision(const GameInfo &gInfo);

protected:
    bool initSprites();
    virtual void collisionResponse(obj_t withtype, ZirconRect overlap_r);
    void updatePosition();
    void updateState();
};

#endif