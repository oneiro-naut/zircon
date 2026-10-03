#ifndef ZIRCON_SDL_RENDERER_H
#define ZIRCON_SDL_RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Sprite.h"
#include "Renderer.h"
#include "SDLTextureManager.h"

class SDLRenderer : public Renderer
{ // it can render Sprite objects and maybe text
public:
  SDLRenderer(SDL_Window *win, SDL_Rect scrn);
  virtual ~SDLRenderer();
  void clear() const override;
  bool initComplete() override; // notifies about renderer init status to Window class
  void changeScreen(ZirconRect newscreen) override;
  void renderSprite(Sprite *sprite, float x, float y) override; // reference or ptr idk man
  void renderSprite(Sprite *sprite, float x, float y, float scale) override;
  void renderSprite(Sprite *sprite, float x, float y, float scale, SDL_RendererFlip flip);
  void renderSprite(Sprite *sprite, float x, float y, float scale, double angle, SDL_Point *center, SDL_RendererFlip flip);
  void renderText(char *text, int x, int y, TTF_Font *font, SDL_Color color); // char * rules because i m illiterate when it comes to string...well
  void renderTexture(Texture texture, ZirconRect srcrect, int x, int y, float scale) override;
  // idk how to create a formatted std::string so...sprintf anyday ;]
  SDL_Texture *createTexturefromSurface(SDL_Surface *surface);
  SDL_Renderer *getRenderer() { return _renderer; }

  TextureManager &getTextureManager() override { return m_textureManager; }

private:
  SDL_Window *pwin;        // associated window (not just a reference)
  SDL_Rect screen;         // camera basically
  SDL_Renderer *_renderer; // will point to window renderer here
  SDLTextureManager m_textureManager;

  void createRenderer();
  void apply_text(SDL_Surface *surface, SDL_Rect position);
  SDL_Rect getDestinationRect(float x, float y, int width, int height, float scale);

  SDL_Texture *m_scoreTextTexture{nullptr}; // not using unique ptr for now.
  bool isInsideScreen(SDL_Rect r);
};

#endif // ZIRCON_SDL_RENDERER_H