#ifndef ENEMY_H
#define ENEMY_H
#include "Object.h"
class Game;
//class Object;

class Enemy : public Object{
    public:
    Enemy(Game& g,int l,float x,float y,SDL_Texture* sprt);
    ~Enemy();
    virtual void hasCollided(obj_t withtype,SDL_Rect overlap_r);
    virtual void update();
    void pollEvents();//its not needed anymore

    protected:
    Uint32 ebul_timer;
    enum State{IDLE,FIRE,HIT,DEAD,ANIM_SIZE}state=IDLE;
    bool shield;
    virtual void collisionResponse(obj_t withtype,SDL_Rect overlap_r);
    virtual void checkBoundaryCollision();
    virtual SDL_Rect getCurrentSprite();
    bool initSprites();
    void updateByKey();
    virtual SDL_Rect getNextFrame();
    void changeState(State nextstate);
    void updateSprite(bool change);
    void activateShield();
    bool shielded();
    void fireBullet();
    void updatePosition();



};



#endif