#ifndef ZIRCON_TEXTURE_MANAGER_H
#define ZIRCON_TEXTURE_MANAGER_H

#include <memory>
#include <string>
#include "Texture.h"

// class TextureManagerImpl;

// Ideally should be a singleton, but lets skip that for now
// Load and manage all textures lifecycle in-memory
//
class TextureManager
{
public:
    TextureManager();
    virtual ~TextureManager();

    virtual Texture loadTexture(const std::string &path) = 0; // for simplicity
    virtual bool unloadTexture(int handle) = 0;

protected:
};

#endif // ZIRCON_TEXTURE_MANAGER_H
