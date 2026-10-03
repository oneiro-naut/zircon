#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
#include "Timer.h"

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
    Enemy(obj_t t, int l, float x, float y, Texture sprt);
    ~Enemy();
    virtual void hasCollided(obj_t withtype, ZirconRect overlap_r);
    virtual void update();
    virtual void checkBoundaryCollision(const GameInfo &gInfo);

protected:
    Timer m_bulletTimer;
    virtual void collisionResponse(obj_t withtype, ZirconRect overlap_r);
    bool initSprites();
    void fireBullet();
    void updateState();
    void updatePosition();
};

#endif