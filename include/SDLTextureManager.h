#ifndef ZIRCON_SDL_TEXTURE_MANAGER_H
#define ZIRCON_SDL_TEXTURE_MANAGER_H

#include "TextureManager.h"
#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include <memory>
//#include "SDLRenderer.h"

// class SDLRenderer; // resolves circular dependency between Renderer and this class
/*
    This manager supports 1 Renderer mapping only
    Each texture here is associated to 1 renderer.
*/

// Resolved it now SDLTextureManager is a part of SDLRenderer owned by it

class SDLTextureManager : public TextureManager
{
public:
    SDLTextureManager(SDL_Renderer *renderer);

    // void registerRenderer(SDLRenderer *renderer); // workaround becoz of circular dependency between the 2
    //  looks like texture manager would fit better as something which is a part of renderer itself but anyway
    virtual ~SDLTextureManager();

    Texture loadTexture(const std::string &path) override;

    // does not destroy the surface :)
    Texture loadTexture(SDL_Surface *surface);

    SDL_Texture *getTexture(int handle); // magic // only renderer will use this method

    bool unloadTexture(int handle) override;

private:
    std::vector<SDL_Texture *> m_textureStore; // index = handle
    std::queue<int> m_availableIndices;        // null vector indices available to be used
    SDL_Renderer *m_renderer;

private:
    SDL_Texture *createTextureFromSurface(SDL_Surface *surface);

    void storeTexture(SDL_Texture *texture, Texture &textureView);

    int getAvailableIndexFromQueue();
};

#endif // ZIRCON_SDL_TEXTURE_MANAGER_H