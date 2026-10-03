#ifndef SPRITE_H
#define SPRITE_H

#include "Timer.h"
#include "Texture.h"
#include "Rectangle.h"
// Assumes that the image/texture passed stores the sprite animation frames as below
/*

[frame 1][frame 2][frame 3].....[frame N] all the animation frame must be in same row in the image/spritesheet
here each frame is same width and height, frames are packed in sequence in the image
so that we could add width to the x coord of current frame to get to the next animation frame.
if the image layout is not like this, this class will not work

Usually old school spritesheets would suffice.
*/
class Sprite
{
public:
  Sprite(int n, int c, int fps, ZirconRect base, Texture img);
  ~Sprite();
  ZirconRect getCurrentFrame();
  Texture getTexture() { return m_img; }
  bool isOver() { return m_over; }
  void update();
  void stop(); // will internally reset the curr animation state

private:
  // SDL_RenderFlip FLIP;
  bool m_over;
  int m_frameCount;   // current frame
  int m_totalFrames;  // total frames
  int m_loopCount;    // loop count
  int m_maxLoopCount; // max no of loops -1 for indefinite
  int m_fps;          // animation speed NOTE: THIS WILL ONLY WORK TO SLOW DOWN THE ANIMATION COMPARED TO THE GAME FRAME RATE(30 in our case)
  Timer m_clock;
  ZirconRect m_baseFrame; // base frame
  ZirconRect m_currFrame; // current frame
  // SDL_Texture *m_img;   // image to render
  Texture m_img;
  // yes it can draw they have access to the global renderer
  // we wont call clear screen from here tho ...only apply render should be called
  void reset();
  void updateFrame();
};

#endif