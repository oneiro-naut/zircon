#include "Player.h"
#include <iostream>

Player::Player(float x, float y, float vx, float vy, obj_t t, int l, int w, int h, Texture sprt) : Object(t, l, w, h, sprt), m_bulletTimer(200), m_shieldTimer(5000)
{
    m_shield = false;
    _x = x;
    _y = y;
    _vx = 0;
    _vy = 0;
    m_maxVx = vx;
    m_maxVy = vy;
    _ax = 0;
    _ay = 0;
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
        _vy += m_maxVy;
    }
    if (m_inputState->keyState[enumClassindex(Key::Up)] == 1)
    {
        _vy += -m_maxVy;
    }
    if (m_inputState->keyState[enumClassindex(Key::Right)] == 1)
    {
        _vx += m_maxVx;
    }
    if (m_inputState->keyState[enumClassindex(Key::Left)] == 1)
    {
        _vx += -m_maxVx;
    }
    if (m_inputState->keyState[enumClassindex(Key::Space)] == 1) // not suitable for this tap like event where holding does not make sense you press it once to one bullet
    {
        fireBullet();
        return;
    }
}

void Player::update()
{
    if (!isAlive())
    {
        return;
    }
    updateByKey();
    updateShield();
    updateSpriteFrame();
    updateState();
}

void Player::fireBullet()
{
    if (m_bulletTimer.updateTickOnTimeout())
    {
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
