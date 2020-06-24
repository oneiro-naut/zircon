#ifndef BULLET_H
#define BULLET_H
#include "Object.h"

//class Object;
class Game;

class Bullet : public Object{
    public:
    Bullet(Game& g,obj_t btype,float x,float y,float vx,float vy,SDL_Texture* sprt);
    virtual void update();
    ~Bullet();
    virtual void hasCollided(obj_t withtype,SDL_Rect overlap_r);
    protected:
    bool initSprites();
    virtual void collisionResponse(obj_t withtype,SDL_Rect overlap_r);
    void updatePosition();
    void updateState();
    virtual void checkBoundaryCollision();
  
   
};

#endif