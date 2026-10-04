#ifndef ZIRCON_LEVEL_MANAGER_H
#define ZIRCON_LEVEL_MANAGER_H

#include <string>
#include <vector>

#include "Types.h"

// this will be fucking ugly looking
struct ObjectCtx
{
    std::string type; // object type
    // spawn position n velocity
    int life;
    float x, y;
    float vx, vy;
    std::string textureId;
    // id?
};

struct ObjectCreationCtx
{
    std::vector<ObjectCtx> objects;
};

// Will give me what
class LevelLoader
{
public:
    LevelLoader(const std::string &path);
    ~LevelLoader();

    const ObjectCreationCtx &getObjectCreationCtx() const { return m_objCtx; }

private:
    std::string m_levelFile;
    ObjectCreationCtx m_objCtx;

    int loadAndParseObjCtx();
};

#endif // ZIRCON_LEVEL_MANAGER_H