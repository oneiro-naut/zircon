#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Sprite.h"

class Renderer
{ // it can render Sprite objects and maybe text
public:
  Renderer(SDL_Window *win, SDL_Rect scrn);
  ~Renderer();
  void clear() const;
  bool initComplete(); // notifies about renderer init status to Window class
  void changeScreen(SDL_Rect newscreen);
  void renderSprite(Sprite *sprite, float x, float y); // reference or ptr idk man
  void renderSprite(Sprite *sprite, float x, float y, float scale);
  void renderSprite(Sprite *sprite, float x, float y, float scale, SDL_RendererFlip flip);
  void renderSprite(Sprite *sprite, float x, float y, float scale, double angle, SDL_Point *center, SDL_RendererFlip flip);
  void renderText(char *text, int x, int y, TTF_Font *font, SDL_Color color); // char * rules because i m illiterate when it comes to string...well
  void renderTexture(SDL_Texture *texture, SDL_Rect srcrect, int x, int y, float scale);
  // idk how to create a formatted std::string so...sprintf anyday ;]
  SDL_Texture *createTexturefromSurface(SDL_Surface *surface);

private:
  SDL_Window *pwin;        // associated window (not just a reference)
  SDL_Rect screen;         // camera basically
  SDL_Renderer *_renderer; // will point to window renderer here
  void createRenderer();
  void apply_text(SDL_Surface *surface, SDL_Rect position);
  SDL_Rect getDestinationRect(float x, float y, int width, int height, float scale);

  bool isInsideScreen(SDL_Rect r);
};

#endif