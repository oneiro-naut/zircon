#include "Player.h"
#include <iostream>

Player::Player(Game& g,int l,SDL_Texture* sprt):Object(g,l,sprt)
{
    pbul_timer = 0;
    shield_timer = 0;
    type = PLAYER;
    shield = false;
    _x = 0;
    _y = game.camera.h / 2;
    _vx = 0;
    _vy = 0;
    _ax = 0;
    _ay = 0;   
    //alive = true;
    sprites = new anima_t* [ANIM_SIZE];
    state = IDLE;
    if(!initSprites())
    {
        life = 0;
    }



}

Player::~Player()
{

}

bool Player::initSprites()//can be parsed from a file 
{
    
    SDL_Rect idle = {192,32,32,32};//can store these offsets in a file to parse
    
    //sdl rect up sdl rect down...
    sprites[IDLE] = new anima_t(4,0,idle);
    sprites[FIRE] = sprites[IDLE];
    sprites[HIT] = sprites[IDLE];
    sprites[DEAD] = sprites[IDLE];
    curr_sprite = *(sprites[state]);
    return true;
}

void Player::pollEvents()
{
    //is it even needed? no
}

void Player::updateByKey()
{
    const Uint8* keyarr = game.keystate;
    setvX(0);
    setvY(0);
    if(keyarr[SDL_SCANCODE_C]==1)
    {
        _vy += 3;
    }
    if(keyarr[SDL_SCANCODE_A]==1)
    {
        _vy += -3;
    }
    if(keyarr[SDL_SCANCODE_D]==1)
    {
        _vx += 3;
    }
    if(keyarr[SDL_SCANCODE_Z]==1)
    {
        _vx += -3;
    }    
    if(keyarr[SDL_SCANCODE_RCTRL]==1)
    {
        fireBullet();
        changeState(FIRE);
        return;
    }
    changeState(IDLE);
}


void Player::update()
{
    if(!isAlive())
    {
        return;
    }
    updateByKey();
    updatePosition();
    curr_frame = getNextFrame();
    
}

void Player::updatePosition()
{

    updateX();
    updateY();
    checkBoundaryCollision();
}


void Player::fireBullet()
{
 
    if(pbul_timer==0||SDL_GetTicks()>pbul_timer)
    {
        pbul_timer = SDL_GetTicks() + 200; // bullet generation delay in ms
        //cout<<"player y = "<<_y<<"bullet y = "<<_y+(curr_frame.h/4)<<endl;
        game.genPBullet(_x+curr_frame.w-8,_y+(curr_frame.h/4));
    }
    
}


bool Player::shielded()
{
   if(shield){
    if(!shield_timer)
    {
        shield_timer = SDL_GetTicks() + 1000;//1s shield
    }
    else if(SDL_GetTicks()<=shield_timer)
    {
        //cout <<"Shield active"<<endl;
        return shield;

    }
    else{
        shield_timer = 0;
        shield = false;
        }
    }
    
    return shield;

}

void Player::activateShield()
{
    shield = true;
}

void Player::collisionResponse(obj_t withtype,SDL_Rect overlap_r)
{
    
    switch(withtype)
    {
        case EBULLET:
        if(!shielded()){
            life--;
            cout<<"life = "<<life<<endl;
            if(isAlive())activateShield();
        }
        break;
        case ENEMY:
        if(!shielded()){
            life--;
            if(isAlive())activateShield();
        }
        break;
        case POWERUP:
        life++;
        break;

    }
    

}


void Player::updateSprite(bool change)
{
    if(change == true)
    {
        curr_sprite = *(sprites[state]);
    }    
    
}

void Player::changeState(State nextstate)
{
    bool changed = true;
    if(nextstate == state)
    {
        changed = false;
    }   
    state = nextstate;
    updateSprite(changed);
    
}


SDL_Rect Player::getNextFrame()
{
    //static Uint32 timer = 0;
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

SDL_Rect Player::getCurrentSprite()//amisnomer
{
    return curr_frame;
}


void Player::checkBoundaryCollision()
{
    if(_x < 0 )
    {
        setX(0);
        
    }
    if(_x+curr_frame.w > game.camera.w)
    {
        setX(game.camera.w - curr_frame.w);
        //state = DEAD;
    }
    if(_y < 0)
    {
        setY(0);
    }
    if(_y+curr_frame.h > game.camera.h)
    {
        setY(game.camera.h - curr_frame.h);
    }


}

void Player::hasCollided(obj_t withtype,SDL_Rect overlap_r)
{
    collisionResponse(withtype,overlap_r);
}