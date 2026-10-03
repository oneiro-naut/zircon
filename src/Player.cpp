#include "Player.h"
#include <iostream>

Player::Player(float x, float y, obj_t t, int l, int w, int h, Texture sprt) : Object(t, l, w, h, sprt), m_bulletTimer(200), m_shieldTimer(5000)
{
    // pbul_timer = 0;
    // shield_timer = 0;
    //  type = PLAYER;
    m_shield = false;
    _x = x;
    _y = y;
    _vx = 0;
    _vy = 0;
    _ax = 0;
    _ay = 0;
    // alive = true;
    if (!initSprites())
    {
        life = 0;
    }
}

Player::~Player()
{
}

bool Player::initSprites() // can be parsed from a file
{

    ZirconRect idle = {192, 32, 32, 32}; // can store these offsets in a file to parse
    ZirconRect sh = {320, 32, 32, 32};
    // sdl rect up sdl rect down...
    state = "idle";
    addSprite(state, 4, -1, 10, idle, sheet);
    addSprite("shield", 4, -1, 10, sh, sheet);
    curSprite = spriteset[state];
    return true;
}

void Player::updateByKey()
{
    setvX(0);
    setvY(0);
    if (m_inputState->keyState[enumClassindex(Key::Down)] == 1)
    {
        _vy += 15;
    }
    if (m_inputState->keyState[enumClassindex(Key::Up)] == 1)
    {
        _vy += -15;
    }
    if (m_inputState->keyState[enumClassindex(Key::Right)] == 1)
    {
        _vx += 15;
    }
    if (m_inputState->keyState[enumClassindex(Key::Left)] == 1)
    {
        _vx += -15;
    }
    if (m_inputState->keyState[enumClassindex(Key::Space)] == 1) // not suitable for this tap like event where holding does not make sense you press it once to one bullet
    {
        fireBullet();
        // changeState(FIRE);
        return;
    }
    // changeState(IDLE);
}

void Player::update()
{
    if (!isAlive())
    {
        return;
    }
    updateByKey();
    // updatePosition();
    updateShield();
    updateSpriteFrame();
    updateState();
}

void Player::updatePosition()
{
    updateX();
    updateY();
    // checkBoundaryCollision();
}

void Player::fireBullet()
{

    // if (pbul_timer == 0 || SDL_GetTicks() > pbul_timer)
    if (m_bulletTimer.updateTickOnTimeout())
    {
        // pbul_timer = SDL_GetTicks() + 200; // bullet generation delay in ms

        PFireBulletMessage pfbMsg(_x, _y, _w, _h);
        dispatchEvent(&pfbMsg);
    }
}

void Player::updateShield()
{
    shielded();
}

bool Player::shielded()
{
    if (m_shield)
    {
        if (m_shieldTimer.resetTickOnTimeout()) // shield timer started
        {
            // shield_timer = SDL_GetTicks() + 5000; // 5s shield
            m_shield = false;
            changeState("idle");
            std::cout << "Shield deactivated!" << std::endl;
        }
    }

    return m_shield;
}

void Player::activateShield()
{
    m_shield = true;
    std::cout << "Shield activated" << std::endl;
    changeState("shield");
}

void Player::collisionResponse(obj_t withtype, ZirconRect overlap_r)
{

    switch (withtype)
    {
    case EBULLET:
        if (!shielded())
        {
            life--;
            std::cout << "life = " << life << std::endl;
            if (isAlive())
                activateShield();
        }
        break;
    case ENEMY:
        if (!shielded())
        {
            life--;
            if (isAlive())
                activateShield();
        }
        break;
    case POWERUP:
        life++;
        break;
    }
}

void Player::checkBoundaryCollision(const GameInfo &gInfo)
{
    if (_x < 0)
    {
        setX(0);
    }
    if (_x + _w > gInfo.sceneWidth)
    {
        setX(gInfo.sceneWidth - _w);
        // state = DEAD;
    }
    if (_y < 0)
    {
        setY(0);
    }
    if (_y + _h > gInfo.sceneHeight)
    {
        setY(gInfo.sceneHeight - _h);
    }
}

void Player::updateState()
{
    if (curSprite->isOver())
    {
        if (!strcmp(state, "shield"))
            changeState("idle");
    }
}

void Player::hasCollided(obj_t withtype, ZirconRect overlap_r)
{
    collisionResponse(withtype, overlap_r);
}