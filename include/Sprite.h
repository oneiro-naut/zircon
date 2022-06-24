#ifndef SPRITE_H
#define SPRITE_H
#include <SDL2/SDL.h>

class Sprite
{
public:
  Sprite(int n, int c, int fps, SDL_Rect base, SDL_Texture *img);
  ~Sprite();
  SDL_Rect getCurrentFrame();
  SDL_Texture *getTexture();
  bool isOver() { return _OVER; }
  void update();
  void draw();
  void stop(); // will internally reset the curr animation state
private:
  // SDL_RenderFlip FLIP;
  bool _OVER;
  int FRAME_COUNT; // current frame
  int N_FRAMES;    // total frames
  int COUNT;       // loop count
  int MAX_COUNT;   // max no of loops -1 for indefinite
  int FPS;         // animation speed NOTE: THIS WILL ONLY WORK TO SLOW DOWN THE ANIMATION COMPARED TO THE GAME FRAME RATE(30 in our case)
  Uint32 CLOCK;
  SDL_Rect BASE_FRAME; // base frame
  SDL_Rect CUR_FRAME;  // current frame
  SDL_Texture *IMG;    // image to render
  // yes it can draw they have access to the global renderer
  // we wont call clear screen from here tho ...only apply render should be called
  void resetTimer();
  void startTimer();
  void reset();
  void animate();
  void start();
  void updateFrame();
  void updateClock();
};

#endif