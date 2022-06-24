#ifndef PLAYER_H
#define PLAYER_H
#include "Object.h"

class PFireBulletMessage : public Message
{
public:
    PFireBulletMessage(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { code = 3; }
    float x, y, w, h;
};

class Player : public Object
{
public:
    Player(const GameInfo &gInfo, obj_t t, int l, int w, int h, SDL_Texture *sprt);
    ~Player();
    virtual void hasCollided(obj_t withtype, SDL_Rect overlap_r);
    virtual void update();
    // void draw();//lets see...it resides in the base class Object, well it doesnt...removed it
    void pollEvents(); // its not needed anymore
protected:
    Uint32 pbul_timer;
    Uint32 shield_timer;
    void updateShield();
    void updateState();
    bool shield;
    virtual void collisionResponse(obj_t withtype, SDL_Rect overlap_r);
    virtual void checkBoundaryCollision();
    bool initSprites();
    void updateByKey();
    void updateSprite(bool change);
    void activateShield();
    bool shielded();
    void fireBullet();
    void updatePosition();
};
#endif