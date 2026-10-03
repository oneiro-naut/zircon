#include "SDLTextureManager.h"

#include <iostream>
#include <SDL2/SDL_image.h>

SDLTextureManager::SDLTextureManager(SDL_Renderer *renderer) : m_renderer(renderer)
{
}

SDLTextureManager::~SDLTextureManager()
{
    for (auto &texture : m_textureStore)
        if (texture)
            SDL_DestroyTexture(texture);
    m_textureStore.clear();
    m_availableIndices = {};
}

Texture SDLTextureManager::loadTexture(const std::string &path)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    Texture textureView;
    textureView.m_handle = -1; // invalid texture

    if (path.empty())
    {
        fprintf(stderr, "[%s: %d]Warning: image string NULL\n", __FILE__, __LINE__);
        return textureView;
    }

    surface = IMG_Load(path.c_str());

    if (surface == NULL)
    {
        // fprintf(stderr, "[%s: %d]Warning: Could not load image %s into surface, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return textureView;
    }
    if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0)))
    {
        SDL_FreeSurface(surface);
        // fprintf(stderr, "[%s: %d]Warning: Could not set color key for image %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return textureView;
    }
    texture = createTextureFromSurface(surface);
    SDL_FreeSurface(surface);

    if (texture == NULL)
    {
        // fprintf(stderr, "[%s: %d]Warning: Could not create texture %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return textureView;
    }

    // add texture to texture store
    storeTexture(texture, textureView);

    return textureView;
}

void SDLTextureManager::storeTexture(SDL_Texture *texture, Texture &textureView)
{
    int index = -1;

    if (texture == NULL)
    {
        textureView.m_handle = index;
        return;
    }

    // set texture resolution to view
    SDL_QueryTexture(texture, nullptr, nullptr, &(textureView.m_width), &(textureView.m_height));

    // check available index in the queue
    index = getAvailableIndexFromQueue();

    if (index >= 0) // valid index found
    {
        textureView.m_handle = index;
        return;
    }

    // do a push back to the store
    m_textureStore.push_back(texture);
    textureView.m_handle = m_textureStore.size() - 1;

    return;
}

int SDLTextureManager::getAvailableIndexFromQueue()
{
    if (m_availableIndices.empty())
    {
        return -1;
    }

    int index = m_availableIndices.front();
    m_availableIndices.pop();

    return index;
}

SDL_Texture *SDLTextureManager::createTextureFromSurface(SDL_Surface *surface)
{
    SDL_Texture *t = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (t == nullptr)
    {
        std::cerr << "Couldnt create texture " << std::endl;
    }
    return t;
}

Texture SDLTextureManager::loadTexture(SDL_Surface *surface)
{
    SDL_Texture *texture = NULL;
    Texture textureView;
    textureView.m_handle = -1; // invalid texture

    if (surface == nullptr)
    {
        return textureView;
    }

    texture = createTextureFromSurface(surface);

    if (texture == NULL)
    {
        // fprintf(stderr, "[%s: %d]Warning: Could not create texture %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return textureView;
    }

    // add texture to texture store
    storeTexture(texture, textureView);

    return textureView;
}

SDL_Texture *SDLTextureManager::getTexture(int handle) // magic // only renderer will use this method
{
    if (handle >= 0 && handle < m_textureStore.size())
        return m_textureStore[handle];

    return nullptr;
}

bool SDLTextureManager::unloadTexture(int handle)
{
    if (handle < 0 || handle >= m_textureStore.size()) // invalid index range
        return false;

    SDL_Texture *texture = m_textureStore[handle];

    SDL_DestroyTexture(texture);

    m_textureStore[handle] = nullptr;
    m_availableIndices.push(handle);

    return true;
}
