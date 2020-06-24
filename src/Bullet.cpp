#include "Bullet.h"
#include <iostream>
Bullet::Bullet(Game& g,obj_t btype,float x,float y,float vx,float vy,SDL_Texture* sprt)
:Object(g,btype,x,y,vx,vy,sprt)
{
    life = 1;
    _w = 16;
    _h = 16;
    _ax = 0;
    _ay = 0;   
    //alive = true;
    if(!initSprites())
    {
        life = 0;
    }


}


bool Bullet::initSprites()//can be parsed from a file 
{
    
    SDL_Rect alivep = {32,144,16,16};//can store these offsets in a file to parse
    SDL_Rect alivee = {32,144,16,16};
    SDL_Rect hitb = {96,144,16,16};
    //sdl rect up sdl rect down...
    state = "alive";
    if(type==PBULLET)addSprite("alive",4,-1,15,alivep,sheet);
    if(type==EBULLET)addSprite("alive",4,-1,15,alivee,sheet);
    addSprite("hit",6,1,20,hitb,sheet);
    addSprite("dead",6,0,20,hitb,sheet);
    curSprite = spriteset[state];
    
    return true;
}



void Bullet::update()
{
    if(!isAlive())
    {
        return;
    }
   
    
    updatePosition();
    updateSpriteFrame();
    updateState();
    
}

void Bullet::updatePosition()
{
    updateX();
    updateY();
    checkBoundaryCollision();//can kill it
}




void Bullet::collisionResponse(obj_t withtype,SDL_Rect overlap_r)
{
    
    switch(withtype)
    {
        case EBULLET:
            if(type==PBULLET)
            {
                //life--;
            changeState("hit");
            }
            break;
        case PBULLET:
            if(type==EBULLET)
            {
                //life--;
            changeState("hit");
            }
            break;
        case ENEMY:
            if(type==PBULLET)
            {
                //life--;
            changeState("hit");
            }
            break;
        case PLAYER:
            if(type==EBULLET)
            {
                //life--;
                changeState("hit");
            }
            break;
        };


    
    

}



void Bullet::updateState()
{
    if(curSprite->isOver())
    {
        if(!strcmp(state,"hit"))changeState("dead");
    }
}



void Bullet::checkBoundaryCollision()
{
    if(_x < 0 )
    {
        if(type==EBULLET)life--;
        if(!isAlive())changeState("dead");
    }
    else if(_x+_w > game.camera.w)
    {
        if(type==PBULLET)life--;
        if(!isAlive())changeState("dead");
    }
    else if(_y < 0)
    {
        life--;
        if(!isAlive())changeState("dead");
    }
    else if(_y+_h > game.camera.h)
    {
        life--;
        if(!isAlive())changeState("dead");
    }


}

void Bullet::hasCollided(obj_t withtype,SDL_Rect overlap_r)
{
    collisionResponse(withtype,overlap_r);
}


Bullet::~Bullet()
{
    
}