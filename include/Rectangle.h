#ifndef RECT_H
#define RECT_H

#include <SDL2/SDL.h>

typedef struct ZirconRect
{
    int x, y;
    int w, h;
} ZirconRect;

// sorry but no class shit here /// ok
ZirconRect createRectangle(int x, int y, int w, int h);
int inline getRectArea(ZirconRect r) { return r.w * r.h; };
ZirconRect getOverlapRect(ZirconRect r1, ZirconRect r2);

#endif