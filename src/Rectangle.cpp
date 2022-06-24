#include "Rectangle.h"

SDL_Rect createRectangle(int x, int y, int w, int h)
{
    SDL_Rect r = {0, 0, 0, 0};
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    return r;
}

SDL_Rect getOverlapRect(SDL_Rect r1, SDL_Rect r2) // found it physics n-body-simulation
{
    int x0 = r1.x;
    int y0 = r1.y;
    int w0 = r1.w;
    int h0 = r1.h;
    int x1 = r2.x;
    int y1 = r2.y;
    int w1 = r2.w;
    int h1 = r2.h;

    int interx1 = 0;
    int interx2 = 0;
    int intery1 = 0;
    int intery2 = 0;

    if (x0 < x1 && x1 < x0 + w0)
    {
        interx1 = x1;
    }
    if (x0 < x1 + w1 && x1 + w1 < x0 + w0)
    {
        interx2 = x1 + w1;
    }
    if (x1 < x0 && x0 < x1 + w1)
    {
        interx1 = x0;
    }
    if (x1 < x0 + w0 && x0 + w0 < x1 + w1)
    {
        interx2 = x0 + w0;
    }
    if (x0 == x1 && w0 == w1)
    {
        interx1 = x0;
        interx2 = x0 + w0;
    }
    if (y0 < y1 && y1 < y0 + h0)
    {
        intery1 = y1;
    }
    if (y0 < y1 + h1 && y1 + h1 < y0 + h0)
    {
        intery2 = y0 + h0;
    }
    if (y1 < y0 && y0 < (y1 + h1))
    {
        intery1 = y0;
    }
    if (y1 < (y0 + h0) && y0 + h0 < (y1 + h1))
    {
        intery2 = y0 + h0;
    }
    if (y0 == y1 && h0 == h1)
    {
        intery1 = y0;
        intery2 = y0 + h0;
    }

    SDL_Rect interrect = {interx1, intery1, interx2 - interx1, intery2 - intery1};
    return interrect;
}
