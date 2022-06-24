#include "Renderer.h"
#include "Rectangle.h"
#include <iostream>

// who will call the constructor tho i think from the Game class idk
Renderer::Renderer(SDL_Window *win, SDL_Rect scrn) : pwin(win), screen(scrn)
{
    createRenderer(); // call it man
}

void Renderer::createRenderer() // can add options here this will be the default tho
{
    _renderer = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);

    if (_renderer == nullptr)
    {
        std::cerr << "Failed to create renderer" << std::endl;
    }
}

bool Renderer::initComplete()
{
    if (_renderer == nullptr)
        return false;
    return true;
}

void Renderer::changeScreen(SDL_Rect newscreen) // i think Window class should call the constructor n should be the one to own the object
{
    screen = newscreen;
    // std::cout << screen.w << "changed screen" << std::endl;
}

void Renderer::renderSprite(Sprite *sprite, float x, float y, float scale, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    // get current frame to render == get source rectangle and image
    SDL_Rect frame = sprite->getCurrentFrame();
    SDL_Texture *src_img = sprite->getTexture();

    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, scale);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, src_img, &frame, &drect, angle, center, flip))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
    // TaDaaa...anyway
}

void Renderer::renderSprite(Sprite *sprite, float x, float y) // simplest
{
    // get current frame to render == get source rectangle and image
    SDL_Rect frame = sprite->getCurrentFrame();
    SDL_Texture *src_img = sprite->getTexture();
    // std::cout <<screen.x<<"renderer screen x"<<std::endl;
    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, 1);
    if (!isInsideScreen(drect))
    {
        // std::cout << "is outisde screen" << std::endl;
        return;
    }

    if (SDL_RenderCopyEx(_renderer, src_img, &frame, &drect, 0, NULL, SDL_FLIP_NONE))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

void Renderer::renderSprite(Sprite *sprite, float x, float y, float scale)
{

    // get current frame to render == get source rectangle and image
    SDL_Rect frame = sprite->getCurrentFrame();
    SDL_Texture *src_img = sprite->getTexture();

    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, scale);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, src_img, &frame, &drect, 0, NULL, SDL_FLIP_NONE))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

void Renderer::renderSprite(Sprite *sprite, float x, float y, float scale, SDL_RendererFlip flip)
{

    // get current frame to render == get source rectangle and image
    SDL_Rect frame = sprite->getCurrentFrame();
    SDL_Texture *src_img = sprite->getTexture();

    // get destination
    SDL_Rect drect = getDestinationRect(x, y, frame.w, frame.h, scale);
    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, src_img, &frame, &drect, 0, NULL, flip))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

bool Renderer::isInsideScreen(SDL_Rect r)
{
    SDL_Rect overlap = getOverlapRect(r, screen);
    if (getRectArea(overlap) != 0)
        return true;
    else
        return false;
}

SDL_Rect Renderer::getDestinationRect(float x, float y, int width, int height, float scale)
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

void Renderer::renderText(char *text, int x, int y, TTF_Font *font, SDL_Color color)
{
    SDL_Surface *message = TTF_RenderText_Solid(font, text, color);
    SDL_Rect drect = {x, y, message->w, message->h};
    apply_text(message, drect);
}

void Renderer::apply_text(SDL_Surface *surface, SDL_Rect destRect)
{
    SDL_Texture *texture = nullptr;

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0));

    texture = SDL_CreateTextureFromSurface(_renderer, surface);

    SDL_RenderCopy(_renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture); // should i?
}

// i think i need a separate class for text

void Renderer::clear() const
{
    SDL_RenderPresent(_renderer);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(_renderer);
}

void Renderer::renderTexture(SDL_Texture *texture, SDL_Rect srcrect, int x, int y, float scale)
{
    SDL_Rect drect = {x, y, static_cast<int>(srcrect.w * scale), static_cast<int>(srcrect.h * scale)};

    if (!isInsideScreen(drect))
    {
        return;
    }

    if (SDL_RenderCopyEx(_renderer, texture, &srcrect, &drect, 0.0, NULL, SDL_FLIP_NONE))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n", __FILE__, __LINE__, SDL_GetError());
    }
}

SDL_Texture *Renderer::createTexturefromSurface(SDL_Surface *surface)
{
    SDL_Texture *t = SDL_CreateTextureFromSurface(_renderer, surface);
    if (t == nullptr)
    {
        std::cerr << "Couldnt create texture " << std::endl;
    }
    return t;
}