#ifndef TYPES_H
#define TYPES_H

// Remove Bullshit types from here

typedef enum obj_t
{
    PLAYER,
    POWERUP,
    WALL,
    ENEMY,
    PBULLET,
    EBULLET,
    SIZE

} obj_t; // f ed up naming by me :), this is not fing C its C++

typedef enum
{
    GLOBAL_TIMER,
    SHIELD_TIMER,
    TIMER_TYPE_SIZE
} timerType;

struct GameInfo
{
    int sceneWidth, sceneHeight;
};

class Message
{
public:
    int code; // int codes for different types
    Message() {}
    virtual ~Message() {}
};

#endif