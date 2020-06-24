#include "Player.h"
#include <iostream>

Player::Player(Game& g,obj_t t,int l,int w,int h,SDL_Texture* sprt):Object(g,t,l,w,h,sprt)
{
    pbul_timer = 0;
    shield_timer = 0;
    //type = PLAYER;
    shield = false;
    _x = 0;
    _y = game.camera.h / 2;
    _vx = 0;
    _vy = 0;
    _ax = 0;
    _ay = 0;   
    //alive = true;
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
    SDL_Rect sh = {320,32,32,32};
    //sdl rect up sdl rect down...
    state = "idle";
    addSprite(state,4,-1,10,idle,sheet);
    addSprite("shield",4,-1,10,sh,sheet);
    curSprite = spriteset[state];
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
        //changeState(FIRE);
        return;
    }
    //changeState(IDLE);
}


void Player::update()
{
    if(!isAlive())
    {
        return;
    }
    updateByKey();
    updatePosition();
    updateShield();
    updateSpriteFrame();
    updateState();
    
    
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
        game.genPBullet(_x+_w-8,_y+(_h/4));
    }
    
}

void Player::updateShield()
{
    shielded();
}


bool Player::shielded()
{
   if(shield){
    if(!shield_timer)
    {
        shield_timer = SDL_GetTicks() + 5000;//5s shield
    }
    else if(SDL_GetTicks()<shield_timer)
    {
        cout <<"Shield active"<<endl;
        return shield;

    }
    else if(SDL_GetTicks()>=shield_timer){
        shield_timer = 0;
        shield = false;
        changeState("idle");
        cout<<"shield deactivated!"<<endl;
        }
    }
    
    return shield;

}

void Player::activateShield()
{
    shield = true;
    //state = SHIELD;
    changeState("shield");
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



void Player::checkBoundaryCollision()
{
    if(_x < 0 )
    {
        setX(0);
        
    }
    if(_x+_w > game.camera.w)
    {
        setX(game.camera.w - _w);
        //state = DEAD;
    }
    if(_y < 0)
    {
        setY(0);
    }
    if(_y+_h > game.camera.h)
    {
        setY(game.camera.h - _h);
    }


}

void Player::updateState()
{
    if(curSprite->isOver())
    {
        if(!strcmp(state,"shield"))changeState("idle");
    }
}



void Player::hasCollided(obj_t withtype,SDL_Rect overlap_r)
{
    collisionResponse(withtype,overlap_r);
}