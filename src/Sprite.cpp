#include "Sprite.h"
#include <iostream>
using namespace std;

Sprite::Sprite(int n, int c, int fps, SDL_Rect base, SDL_Texture *img) : N_FRAMES(n), MAX_COUNT(c), FPS(fps), BASE_FRAME(base), IMG(img)
{
    _OVER = false;
    CUR_FRAME = BASE_FRAME;
    CLOCK = 0;
}

Sprite::~Sprite()
{
}

void Sprite::update()
{
    if (CLOCK <= SDL_GetTicks())
    {
        updateClock();
        updateFrame();
    }
}

void Sprite::updateClock()
{
    // cout << FPS<<endl;
    CLOCK = SDL_GetTicks() + 1000 / FPS;
}

void Sprite::stop()
{
    reset();
}

void Sprite::reset()
{
    // cout << "sprite was reset" << endl;
    CUR_FRAME = BASE_FRAME;
    FRAME_COUNT = 1;
    COUNT = 0;
    CLOCK = 0;
    _OVER = false;
}

void Sprite::updateFrame()
{
    if (_OVER == true)
        return;

    if (FRAME_COUNT >= N_FRAMES)
    {
        COUNT++;
        if (MAX_COUNT == -1)
            FRAME_COUNT = 1; // cylic
        else if (MAX_COUNT > 0)
        {
            if (COUNT >= MAX_COUNT)
            {
                _OVER = true;
                reset();
            }
            FRAME_COUNT = 1;
        }
        CUR_FRAME = BASE_FRAME;
    }
    else
    {
        FRAME_COUNT += 1;
        // cout<<"frame count is = "<<FRAME_COUNT<<endl;
        CUR_FRAME.x += CUR_FRAME.w;
    }
}

SDL_Rect Sprite::getCurrentFrame()
{
    return CUR_FRAME;
}

SDL_Texture *Sprite::getTexture()
{
    return IMG;
}