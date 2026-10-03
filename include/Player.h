#ifndef PLAYER_H
#define PLAYER_H
#include "Object.h"
#include "Timer.h"

class PFireBulletMessage : public Message
{
public:
    PFireBulletMessage(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { code = 3; }
    float x, y, w, h;
};

class Player : public Object
{
public:
    Player(float x, float y, obj_t t, int l, int w, int h, Texture sprt);
    ~Player();
    virtual void update();

    virtual void hasCollided(obj_t withtype, ZirconRect overlap_r);

    virtual void checkBoundaryCollision(const GameInfo &gInfo) override;

protected:
    Timer m_bulletTimer;
    Timer m_shieldTimer;

    bool m_shield;

protected:
    bool initSprites();

    void updateState();
    void updateByKey();
    void updatePosition();
    void updateShield();
    void updateSprite(bool change);

    void activateShield();
    bool shielded();

    void fireBullet();

    virtual void collisionResponse(obj_t withtype, ZirconRect overlap_r);
};
#endif