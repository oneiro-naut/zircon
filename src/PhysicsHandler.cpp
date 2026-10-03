#include "PhysicsHandler.h"
#include "Rectangle.h"

PhysicsHandler::PhysicsHandler() {}

PhysicsHandler::~PhysicsHandler() {}

void PhysicsHandler::updatePhysics(Object &player,
                                   const std::vector<std::unique_ptr<Object>> &enemies,
                                   const std::vector<std::unique_ptr<Object>> &bullets,
                                   const std::vector<std::unique_ptr<Object>> &ebullets,
                                   const std::vector<std::unique_ptr<Object>> &powerups,
                                   const GameInfo &gInfo)
{
    updatePosition(&player, gInfo);

    for (const auto &enemy : enemies)
        updatePosition(enemy.get(), gInfo);

    for (const auto &bullet : bullets)
        updatePosition(bullet.get(), gInfo);

    for (const auto &ebullet : ebullets)
        updatePosition(ebullet.get(), gInfo);

    for (const auto &powerup : powerups)
        updatePosition(powerup.get(), gInfo);

    player.checkBoundaryCollision(gInfo);

    for (const auto &enemy : enemies)
        enemy->checkBoundaryCollision(gInfo);

    for (const auto &bullet : bullets)
        bullet->checkBoundaryCollision(gInfo);

    for (const auto &ebullet : ebullets)
        ebullet->checkBoundaryCollision(gInfo);
}

void PhysicsHandler::updatePosition(Object *o, const GameInfo &gInfo)
{
    // void updateX() { _x = _x + _vx; }
    // void updateY() { _y = _y + _vy; }
    // void updatevX() { _vx = _vx + _ax; }
    // void updatevY() { _vy = _vy + _ay; }
    float _x = o->getX();
    float _y = o->getY();
    float _vx = o->getvX();
    float _vy = o->getvY();
    float _ax = o->getaX();
    float _ay = o->getaY();
    // updateX();
    _x = _x + _vx;
    // updateY();
    _y = _y + _vy;

    o->setX(_x);
    o->setY(_y);
    // checkBoundaryCollision();
}

ZirconRect PhysicsHandler::positionObjFrame(Object *o, float scale)
{
    ZirconRect rect = {
        static_cast<int>(o->getX()),
        static_cast<int>(o->getY()),
        static_cast<int>(o->getW() * scale),
        static_cast<int>(o->getH() * scale)};

    return rect;
}

void PhysicsHandler::checkCollision(Object *obj1, Object *obj2)
{
    // more details in copy
    // we can use dynamic programming here n memoiz some stuff or maybe not
    ZirconRect r1 = positionObjFrame(obj1, 1);
    ZirconRect r2 = positionObjFrame(obj2, 1); // scale = 1

    ZirconRect inter = getOverlapRect(r1, r2);
    int area = 0;
    area = getRectArea(inter);

    if (area == 0)
        return; // no collision
    else if (area > 0)
    { // collision occured
        // notify the pair
        obj1->hasCollided(obj2->getType(), inter);
        obj2->hasCollided(obj1->getType(), inter);
    } // we r cool now
}

void PhysicsHandler::handleCollisions(Object &player,
                                      const std::vector<std::unique_ptr<Object>> &enemies,
                                      const std::vector<std::unique_ptr<Object>> &bullets,
                                      const std::vector<std::unique_ptr<Object>> &ebullets,
                                      const std::vector<std::unique_ptr<Object>> &powerups)
{
    for (const auto &enemy : enemies)
        checkCollision(&player, enemy.get());

    for (const auto &bullet : ebullets)
        checkCollision(&player, bullet.get());

    for (const auto &powerup : powerups)
        checkCollision(&player, powerup.get());

    // player-enemy pairs
    for (const auto &enemy : enemies)
    {
        checkCollision(&player, enemy.get());
    }

    // player-ebullet pairs
    for (const auto &ebullet : ebullets)
    {
        checkCollision(&player, ebullet.get());
    }

    // enemy-enemy pairs
    for (auto it = enemies.begin(); it != enemies.end(); ++it)
    {
        for (auto iter = std::next(it); iter != enemies.end(); ++iter)
        {
            checkCollision(it->get(), iter->get());
        }
    }

    // player-powerup pairs
    for (const auto &powerup : powerups)
    {
        checkCollision(&player, powerup.get());
    }

    // enemy-pbullet pairs
    for (const auto &enemy : enemies)
    {
        for (const auto &bullet : bullets)
        {
            checkCollision(enemy.get(), bullet.get());
        }
    }

    // pbullet-ebullet pairs
    for (const auto &bullet : bullets)
    {
        for (const auto &ebullet : ebullets)
        {
            checkCollision(bullet.get(), ebullet.get());
        }
    }
}