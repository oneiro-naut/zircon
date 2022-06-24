#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"

class EFireBulletMessage : public Message
{
public:
    EFireBulletMessage(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { code = 0; }
    float x, y, w, h;
};

class EnemyCollidedMessage : public Message
{
public:
    EnemyCollidedMessage(obj_t with) : withObj(with) { code = 1; }
    obj_t withObj;
};

class Enemy : public Object
{
public:
    Enemy(const GameInfo &gInfo, obj_t t, int l, float x, float y, SDL_Texture *sprt);
    ~Enemy();
    virtual void hasCollided(obj_t withtype, SDL_Rect overlap_r);
    virtual void update();

protected:
    Uint32 ebul_timer;
    virtual void collisionResponse(obj_t withtype, SDL_Rect overlap_r);
    virtual void checkBoundaryCollision();
    bool initSprites();
    void fireBullet();
    void updateState();
    void updatePosition();
};

#endif