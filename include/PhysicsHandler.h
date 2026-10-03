#ifndef ZIRCON_PHYSICS_HANDLER_H
#define ZIRCON_PHYSICS_HANDLER_H

#include "Object.h"
#include <memory>

class PhysicsHandler
{
public:
    PhysicsHandler();
    virtual ~PhysicsHandler();
    // good example of mutate but not delete or allocate anything just mutate
    void updatePhysics(Object &player,
                       const std::vector<std::unique_ptr<Object>> &enemies,
                       const std::vector<std::unique_ptr<Object>> &bullets,
                       const std::vector<std::unique_ptr<Object>> &ebullets,
                       const std::vector<std::unique_ptr<Object>> &powerups,
                       const GameInfo &gInfo);
    void handleCollisions(Object &player,
                          const std::vector<std::unique_ptr<Object>> &enemies,
                          const std::vector<std::unique_ptr<Object>> &bullets,
                          const std::vector<std::unique_ptr<Object>> &ebullets,
                          const std::vector<std::unique_ptr<Object>> &powerups);

private:
    void checkCollision(Object *obj1, Object *obj2);
    ZirconRect positionObjFrame(Object *o, float scale);
    void updatePosition(Object *o, const GameInfo &gInfo);
};

#endif // ZIRCON_PHYSICS_HANDLER_H