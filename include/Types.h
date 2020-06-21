#ifndef TYPES_H
#define TYPES_H
#include <SDL2/SDL.h>

struct anima_t
{
    int N_FRAMES;
    SDL_Rect _FRAME; //initialized with base frame
    int CUR_FRAME;
    //add int MAX_COUNT and int COUNT basically how many times the animation must loop...
    int MAX_COUNT;
    //it can be indefinite ie -1 or some finite count(for animation like PREDEAD or HIT)
    int COUNT;
    anima_t(){
        N_FRAMES = 0;
        //DEF_FRAME = def_f;
        CUR_FRAME = 1;
        MAX_COUNT = -1;
        COUNT = 0;
        _FRAME = {0,0,0,0}; // set according to tileset texture
    }
    
    anima_t(int n,int count,SDL_Rect f_offset)
    {
        N_FRAMES = n;
        //DEF_FRAME = def_f;
        //DEF_FRAME = 1;
        CUR_FRAME = 1;
        MAX_COUNT = count;
        COUNT = 0;
        _FRAME = f_offset; // set according to tileset texture
    }


};

typedef enum obj_t{
    PLAYER,
    POWERUP,
    WALL,
    ENEMY,
    PBULLET,
    EBULLET,
    SIZE


}obj_t;

typedef enum
{
    GLOBAL_TIMER,
    SHIELD_TIMER,
    TIMER_TYPE_SIZE
} timerType;



#endif