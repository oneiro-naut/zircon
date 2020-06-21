#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Game& g,int l,float x,float y,SDL_Texture* sprt):Object(g,l,sprt)
{
    
    type = ENEMY;
    //shield = false;
    _vx = -1;
    _vy = 0;
    _x = x;
    _y = y;
    _ax = 0;
    _ay = 0;   
    //alive = true;
    ebul_timer = 0;
    sprites = new anima_t* [ANIM_SIZE];
    state = IDLE;
    if(!initSprites())
    {
        life = 0;
    }



}

Enemy::~Enemy()
{

}

bool Enemy::initSprites()//can be parsed from a file 
{
    
    SDL_Rect idle = {192,64,32,32};//can store these offsets in a file to parse
    //sdl rect up sdl rect down...
    sprites[IDLE] = new anima_t(4,0,idle);
    sprites[FIRE] = sprites[IDLE];
    sprites[HIT] = sprites[IDLE];
    sprites[DEAD] = sprites[IDLE];
    curr_sprite = *(sprites[state]);
    return true;
}

void Enemy::pollEvents()
{
    //is it even needed? no
}

void Enemy::update()
{
    if(!isAlive())
    {
        return;
    }
    fireBullet();
    updatePosition();
    //cout<<_x<<" "<<_y<<endl;
    curr_frame = getNextFrame();
    
}

void Enemy::updatePosition()
{

    updateX();
    updateY();
    checkBoundaryCollision();
}


void Enemy::fireBullet()
{
    
    if(ebul_timer==0||SDL_GetTicks()>ebul_timer)
    {
        ebul_timer = SDL_GetTicks() + 2000; // bullet generation delay in ms
        //cout<<"enemy y = "<<_y<<"bullet y = "<<_y+(curr_frame.h/4)<<endl;
        game.genEBullet(_x-curr_frame.w,_y+(curr_frame.h/4));
        changeState(FIRE);
    }
    
}


// bool Enemy::shielded()
// {
//     static Uint32 shield_timer = 0;//can make it class member
//     if(shield){
//     if(!shield_timer)
//     {
//         shield_timer = SDL_GetTicks() + 5000;//5s shield
//     }
//     else if(SDL_GetTicks()<=shield_timer)
//     {
//         return shield;
//     }
//     else{
//         shield_timer = 0;
//         shield = false;
//         }
//     }
    
//     return shield;

// }

// void Enemy::activateShield()
// {
//     shield = true;
// }

void Enemy::collisionResponse(obj_t withtype,SDL_Rect overlap_r)
{
    
    switch(withtype)
    {
 

        case PBULLET:
        life--;
        game.updateScore();
        if(!isAlive())changeState(DEAD);
        break;

    }
    

}


void Enemy::updateSprite(bool change)
{
    if(change == true)
    {
        curr_sprite = *(sprites[state]);
    }    
    
}

void Enemy::changeState(State nextstate)
{
    bool changed = true;
    if(nextstate == state)
    {
        changed = false;
    }   
    state = nextstate;
    updateSprite(changed);
    
}


SDL_Rect Enemy::getNextFrame()
{
    
    SDL_Rect frame = curr_sprite._FRAME;
    int cur_f = curr_sprite.CUR_FRAME;
        if(cur_f >= curr_sprite.N_FRAMES )
        {
            curr_sprite.CUR_FRAME = 0;//cylic

        }
        else{
            frame.x += cur_f*frame.w;
            curr_sprite.CUR_FRAME +=1;
        }
    
    return frame;
}

SDL_Rect Enemy::getCurrentSprite()//amisnomer
{
    return curr_frame;
}


void Enemy::checkBoundaryCollision()
{
    if(_x < 0 )
    {
        life = 0;
        changeState(DEAD);//wont increase score tho
    }
    if(_x+curr_frame.w > game.camera.w)
    {
        //do nothing
        //state = DEAD;
    }
    if(_y < 0)
    {
        _vy = -_vy;
    }
    if(_y+curr_frame.h > game.camera.h)
    {
        _vy = -_vy;
    }


}

void Enemy::hasCollided(obj_t withtype,SDL_Rect overlap_r)
{
    collisionResponse(withtype,overlap_r);
}