#include "Bullet.h"
#include <iostream>
Bullet::Bullet(Game& g,obj_t btype,float x,float y,float vx,float vy,SDL_Texture* sprt)
:Object(g,btype,x,y,vx,vy,sprt)
{
    life = 1;
    _ax = 0;
    _ay = 0;   
    //alive = true;
    sprites = new anima_t* [ANIM_SIZE];
    state = ALIVE;
    if(!initSprites())
    {
        life = 0;
    }



}


bool Bullet::initSprites()//can be parsed from a file 
{
    
    SDL_Rect alive = {32,144,16,16};//can store these offsets in a file to parse
    //sdl rect up sdl rect down...
    sprites[ALIVE] = new anima_t(2,0,alive);
    sprites[HIT] = new anima_t(2,0,alive);
    sprites[DEAD] = new anima_t(2,0,alive);
    curr_sprite = *(sprites[state]);
    return true;
}



void Bullet::update()
{
    if(!isAlive())
    {
        return;
    }
    curr_frame = getNextFrame();
    
    updatePosition();
    
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
                life--;
            changeState(HIT);
            }
            break;
        case PBULLET:
            if(type==EBULLET)
            {
                life--;
            changeState(HIT);
            }
            break;
        case ENEMY:
            if(type==PBULLET)
            {
                life--;
            changeState(HIT);
            }
            break;
        case PLAYER:
            if(type==EBULLET)
            {
                life--;
                changeState(HIT);
            }
            break;
        };


    
    

}


void Bullet::updateSprite(bool change)
{
    if(change == true)
    {
        curr_sprite = *(sprites[state]);
    }    
    
}

void Bullet::changeState(State nextstate)
{
    
    bool changed = true;
    if(nextstate == state)
    {
        changed = false;
    }   
    state = nextstate;
    updateSprite(changed);
}


SDL_Rect Bullet::getNextFrame()
{
    static Uint32 timer = 0;
    SDL_Rect frame = curr_sprite._FRAME;
    int cur_f = curr_sprite.CUR_FRAME;
    if(timer==0||SDL_GetTicks()>timer){
        timer = SDL_GetTicks() + 34;//30fps 
        if(cur_f >= curr_sprite.N_FRAMES)
        {
        
            curr_sprite.CUR_FRAME = 0;//cylic
        }
        else{
            frame.x += cur_f*frame.w;
            curr_sprite.CUR_FRAME +=1;
        }
    }
    else if(SDL_GetTicks()<=timer)
    {
        //do nothing just wait its like delay
    }

    return frame;
}

SDL_Rect Bullet::getCurrentSprite()
{
    return curr_frame;
}

void Bullet::checkBoundaryCollision()
{
    if(_x < 0 )
    {
        if(type==EBULLET)life--;
        if(!isAlive())changeState(DEAD);
    }
    else if(_x+curr_frame.w > game.camera.w)
    {
        if(type==PBULLET)life--;
        if(!isAlive())changeState(DEAD);
    }
    else if(_y < 0)
    {
        life--;
        if(!isAlive())changeState(DEAD);
    }
    else if(_y+curr_frame.h > game.camera.h)
    {
        life--;
        if(!isAlive())changeState(DEAD);
    }


}

void Bullet::hasCollided(obj_t withtype,SDL_Rect overlap_r)
{
    collisionResponse(withtype,overlap_r);
}


Bullet::~Bullet()
{
    
}