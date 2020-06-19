#ifndef _OBJECT_H
#define _OBJECT_H
#include "Types.h"
#include "Game.h"
#include <map>

class Game;

class Object{
    public:
    SDL_Texture* sheet;
    Object(Game &g,int l,SDL_Texture* ss):game(g),life(l),sheet(ss){}
    Object(Game &g,obj_t t,float x,float y,float vx,float vy,SDL_Texture* ss)
    :game(g),type(t),_x(x),_y(y),_vx(vx),_vy(vy),sheet(ss)
    {
        
    }
    ~Object(){}
    bool inline isAlive(){ return (life >0); }
    obj_t inline getType(){ return type;}
    int inline getLives(){ return life; }
    float inline getX(){ return _x;};
    float inline getY(){ return _y;};
    int inline getW(){ return curr_frame.w;}
    int inline getH(){ return curr_frame.h;}
    float inline getvX(){ return _vx;};
    float inline getvY(){ return _vy;};
    float inline getaX(){ return _ax;};
    float inline getaY(){ return _ay;};
    virtual void update() = 0;
    virtual SDL_Rect getCurrentSprite() = 0;
    virtual void hasCollided(obj_t withtype,SDL_Rect overlap_r)=0;//could have passed a special collision struct containing details about collision but its fine for now
    protected:
    Game& game;
    obj_t type;
    //bool alive;
    int life;
    float _x, _y;// these will be static_cast<int>()-ed
    float _vx,_vy;// 
    float _ax,_ay;//
    //an enum called STATE since I wont be using maps for this purpose
    //not defining here since plain enums have no forward declaration capability
    //typedef enum State{...,..,.,,ANIM_SIZE}State;
    anima_t** sprites;//going to be array of animation struct of size ANIM_SIZE
    anima_t curr_sprite;
    SDL_Rect curr_frame;
    
    void updateX(){ _x = _x + _vx; }
    void updateY(){ _y = _y + _vy; }
    void updatevX(){ _vx = _vx + _ax;}
    void updatevY(){ _vy = _vy + _ay;}
    void setvX(float vx){ _vx = vx;}
    void setvY(float vy){ _vy = vy;}
    void setaX(float ax){ _ax = ax;}
    void setaY(float ay){ _ay = ay;}
    void setY(float y){ _y = y;}
    void setX(float x){ _x = x;}
    virtual SDL_Rect getNextFrame() = 0;
    virtual void checkBoundaryCollision() = 0;
    virtual void collisionResponse(obj_t withtype,SDL_Rect overlap_r) = 0;
};
#endif