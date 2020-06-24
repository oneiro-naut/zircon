#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
class Game;
//class Object;

class Enemy : public Object{
    public:
    Enemy(Game& g,obj_t t,int l,float x,float y,SDL_Texture* sprt);   
   ~Enemy();
    virtual void hasCollided(obj_t withtype,SDL_Rect overlap_r);
    virtual void update();
    protected:
    Uint32 ebul_timer;
    virtual void collisionResponse(obj_t withtype,SDL_Rect overlap_r);
    virtual void checkBoundaryCollision();
    bool initSprites();
    void fireBullet();
    void updateState();
    void updatePosition();



};



#endif