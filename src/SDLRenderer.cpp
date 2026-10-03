#include "SDLRenderer.h"
#include "Rectangle.h"
#include <iostream>

SDL_Rect toSDLRect(const ZirconRect &rect)
{
    SDL_Rect result;
    result.x = rect.x;
    result.y = rect.y;
    result.w = rect.w;
    result.h = rect.h;
    return result;
}

ZirconRect toZRect(const SDL_Rect &rect)
{
    ZirconRect result;
    result.x = rect.x;
    result.y = rect.y;
    result.w = rect.w;
    result.h = rect.h;
    return result;
}

// who will call the constructor tho i think from the Game class idk
SDLRenderer::SDLRenderer(SDL_Window *win, SDL_Rect scrn) : _renderer(SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED)),
                                                           pwin(win),
                                                           screen(scrn),
                                                           m_textureManager(_renderer)
{
    // createRenderer(); // call it man
    // TODO: if renderer is nullptr throw an exception
}

void SDLRenderer::createRenderer() // can add options here this will be the default tho
{
    _renderer = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);

    if (_renderer == nullptr)
    {
        std::cerr << "Failed to create renderer" << std::endl;
    }
}

bool SDLRenderer::initComplete()
{
    if (_renderer == nullptr)
        return false;
    return true;
}

void SDLRenderer::changeScreen(ZirconRect newscreen) // i think Window class should call the constructor n should be the one to own the object
{
    screen = toSDLRect(newscreen);
    // std::cout << screen.w << "changed screen" << std::endl;
}

void SDLRenderer::renderSprite(Sprite *sprite, float x, float y, float scale, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    // get current frame to render == get source rectangle and image
    SDL_Rect frame = toSDLRect(sprite->getCurrentFrame());
    Texture srcImg = sprite->getTexture();
    SDL_Texture *srcImgTexture = m_textureManager.getTexture(srcImg.m_handle);

    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, scale);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, srcImgTexture, &frame, &drect, angle, center, flip))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
    // TaDaaa...anyway
}

void SDLRenderer::renderSprite(Sprite *sprite, float x, float y) // simplest
{
    // get current frame to render == get source rectangle and image
    SDL_Rect frame = toSDLRect(sprite->getCurrentFrame());
    Texture srcImg = sprite->getTexture();
    SDL_Texture *srcImgTexture = m_textureManager.getTexture(srcImg.m_handle);

    // std::cout <<screen.x<<"renderer screen x"<<std::endl;
    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, 2);
    if (!isInsideScreen(drect))
    {
        // std::cout << "is outisde screen" << std::endl;
        return;
    }

    if (SDL_RenderCopyEx(_renderer, srcImgTexture, &frame, &drect, 0, NULL, SDL_FLIP_NONE))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

void SDLRenderer::renderSprite(Sprite *sprite, float x, float y, float scale)
{

    // get current frame to render == get source rectangle and image
    SDL_Rect frame = toSDLRect(sprite->getCurrentFrame());
    Texture srcImg = sprite->getTexture();
    SDL_Texture *srcImgTexture = m_textureManager.getTexture(srcImg.m_handle);

    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, scale);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, srcImgTexture, &frame, &drect, 0, NULL, SDL_FLIP_NONE))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

void SDLRenderer::renderSprite(Sprite *sprite, float x, float y, float scale, SDL_RendererFlip flip)
{
    // get current frame to render == get source rectangle and image
    SDL_Rect frame = toSDLRect(sprite->getCurrentFrame());
    Texture srcImg = sprite->getTexture();
    SDL_Texture *srcImgTexture = m_textureManager.getTexture(srcImg.m_handle);

    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, scale);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, srcImgTexture, &frame, &drect, 0, NULL, flip))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

bool SDLRenderer::isInsideScreen(SDL_Rect r)
{
    SDL_Rect overlap = toSDLRect(getOverlapRect(toZRect(r), toZRect(screen)));
    if (getRectArea(toZRect(overlap)) != 0)
        return true;
    else
        return false;
}

SDL_Rect SDLRenderer::getDestinationRect(float x, float y, int width, int height, float scale)
{ // relying on implicit type casting NO
    SDL_Rect drect = {
        static_cast<int>(x),
        static_cast<int>(y),
        width,
        height};

    drect.w *= scale;
    drect.h *= scale;
    return drect;
}

// needs optimisation, is wasting memory, redundant texture creation if text has not changed
void SDLRenderer::renderText(char *text, int x, int y, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *message = TTF_RenderText_Solid(font, text, color);
    SDL_Rect drect = {x, y, message->w, message->h};
    apply_text(message, drect);
}

void SDLRenderer::apply_text(SDL_Surface *surface, SDL_Rect destRect)
{
    SDL_Texture *texture = nullptr;

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0));

    texture = SDL_CreateTextureFromSurface(_renderer, surface);

    SDL_RenderCopy(_renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture); // should i? its unnecessarily creating new texture each time even if the text has not changed. in that case we just need to render without creating new texture.
}

// i think i need a separate class for text

void SDLRenderer::clear() const
{
    SDL_RenderPresent(_renderer);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

SDLRenderer::~SDLRenderer()
{
    SDL_DestroyRenderer(_renderer);
}

void SDLRenderer::renderTexture(Texture texture, ZirconRect srcrect, int x, int y, float scale)
{
    SDL_Rect drect = {x, y, static_cast<int>(srcrect.w * scale), static_cast<int>(srcrect.h * scale)};
    SDL_Texture *t = m_textureManager.getTexture(texture.m_handle);
    SDL_Rect srcRect = toSDLRect(srcrect);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, t, &srcRect, &drect, 0.0, NULL, SDL_FLIP_NONE))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

SDL_Texture *SDLRenderer::createTexturefromSurface(SDL_Surface *surface)
{
    SDL_Texture *t = SDL_CreateTextureFromSurface(_renderer, surface);
    if (t == nullptr)
    {
        std::cerr << "Couldnt create texture " << std::endl;
    }
    return t;
}
