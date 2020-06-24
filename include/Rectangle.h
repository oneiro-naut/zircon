#ifndef RECT_H
#define RECT_H

#include <SDL2/SDL.h>

//sorry but no class shit here
SDL_Rect createRectangle(int x,int y,int w,int h);
int inline getRectArea(SDL_Rect r){ return r.w * r.h;};
SDL_Rect getOverlapRect(SDL_Rect r1,SDL_Rect r2);





#endif