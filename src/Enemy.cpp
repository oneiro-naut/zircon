#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const GameInfo &gInfo, obj_t t, int l, float x, float y, SDL_Texture *sprt) : Object(gInfo, t, l, sprt)
{

    // shield = false;
    _vx = -1;
    _vy = 0;
    _x = x;
    _y = y;
    _w = 32;
    _h = 32;
    _ax = 0;
    _ay = 0;
    // alive = true;
    ebul_timer = 0;

    if (!initSprites())
    {
        life = 0;
    }
}

Enemy::~Enemy()
{
}

bool Enemy::initSprites() // can be parsed from a file
{

    SDL_Rect idle = {192, 64, 32, 32}; // can store these offsets in a file to parse
    // sdl rect up sdl rect down...
    state = "idle";
    addSprite(state, 4, -1, 10, idle, sheet);
    addSprite("fire", 4, 1, 10, idle, sheet);
    addSprite("dead", 4, 1, 10, idle, sheet);
    curSprite = spriteset[state];
    return true;
}

void Enemy::fireBullet()
{

    if (ebul_timer == 0 || SDL_GetTicks() > ebul_timer)
    {
        ebul_timer = SDL_GetTicks() + 2000; // bullet generation delay in ms
        // cout<<"enemy y = "<<_y<<"bullet y = "<<_y+(curr_frame.h/4)<<endl;
        EFireBulletMessage emsg(_x, _y, _w, _h);
        dispatchEvent(&emsg); // game.onEvent(&emsg)
        // changeState("fire");
    }
}

void Enemy::update()
{
    if (!isAlive())
    {
        return;
    }
    fireBullet();
    updatePosition();
    updateSpriteFrame();
    updateState();
}

void Enemy::updatePosition()
{

    updateX();
    updateY();
    checkBoundaryCollision();
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

void Enemy::collisionResponse(obj_t withtype, SDL_Rect overlap_r)
{
    switch (withtype)
    {
    case obj_t::PBULLET:
        life--;
        EnemyCollidedMessage ec(obj_t::PBULLET);
        dispatchEvent(&ec);
        // game.updateScore(); // dis
        if (!isAlive())
            changeState("dead");
        break;
    }
}

void Enemy::updateState()
{
    if (curSprite->isOver())
    {
        if (!strcmp(state, "fire"))
            changeState("idle");
    }
}

void Enemy::checkBoundaryCollision()
{
    if (_x < 0)
    {
        life = 0;
        changeState("dead"); // wont increase score tho
    }                        // create some shared info object dont pass game ref itself this sucks
    if (_x + _w > gInfo.sceneWidth)
    {
        // do nothing
        // state = DEAD;
    }
    if (_y < 0)
    {
        _vy = -_vy;
    }
    if (_y + _h > gInfo.sceneHeight)
    {
        _vy = -_vy;
    }
}

void Enemy::hasCollided(obj_t withtype, SDL_Rect overlap_r)
{
    collisionResponse(withtype, overlap_r);
}