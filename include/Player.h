#ifndef PLAYER_H
#define PLAYER_H
#include "Object.h"

//class Object;
class Game;

class Player : public Object
{
    public:
    Player(Game& g,obj_t t,int l,int w,int h,SDL_Texture* sprt);
    ~Player();
    virtual void hasCollided(obj_t withtype,SDL_Rect overlap_r);
    virtual void update();
    //void draw();//lets see...it resides in the base class Object
    void pollEvents();//its not needed anymore
    protected:
    Uint32 pbul_timer ;
    Uint32 shield_timer;
    void updateShield();
    void updateState();
    bool shield;
    virtual void collisionResponse(obj_t withtype,SDL_Rect overlap_r);
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