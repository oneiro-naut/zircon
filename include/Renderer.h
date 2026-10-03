#ifndef ZIRCON_RENDERER_H
#define ZIRCON_RENDERER_H

#include "Sprite.h"
#include "TextureManager.h"

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    virtual void clear() const = 0;
    virtual TextureManager &getTextureManager() = 0;
    virtual bool initComplete() = 0; // notifies about renderer init status to Window class
    virtual void changeScreen(ZirconRect newscreen) = 0;
    virtual void renderSprite(Sprite *sprite, float x, float y) = 0; // reference or ptr idk man
    virtual void renderSprite(Sprite *sprite, float x, float y, float scale) = 0;
    // virtual void renderSprite(Sprite *sprite, float x, float y, float scale, SDL_RendererFlip flip);
    // virtual void renderSprite(Sprite *sprite, float x, float y, float scale, double angle, SDL_Point *center, SDL_RendererFlip flip);

    // void renderText(char *text, int x, int y, TTF_Font *font, SDL_Color color); // char * rules because i m illiterate when it comes to string...well
    virtual void renderTexture(Texture texture, ZirconRect srcrect, int x, int y, float scale) = 0;
};

#endif // ZIRCON_RENDERER_H