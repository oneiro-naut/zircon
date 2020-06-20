#ifndef PLAYER_H
#define PLAYER_H
#include "Object.h"

//class Object;
class Game;

class Player : public Object
{
    public:
    Player(Game& g,int life,SDL_Texture* sprt);
    ~Player();
    virtual void hasCollided(obj_t withtype,SDL_Rect overlap_r);
    virtual void update();
    void pollEvents();//its not needed anymore

    protected:
    Uint32 pbul_timer ;
    Uint32 shield_timer;
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