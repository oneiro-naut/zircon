#ifndef TYPES_H
#define TYPES_H
#include <SDL2/SDL.h>

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