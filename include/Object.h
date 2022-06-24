#ifndef _OBJECT_H
#define _OBJECT_H
#include <map> //yes its time now no more animation state enums
#include <vector>
#include <functional>
#include <string>

#include "Types.h"
#include "Sprite.h"

class Object
{
public:
    Object(const GameInfo &gInfo, obj_t t, int l, int w, int h, SDL_Texture *ss);
    Object(const GameInfo &gInfo, obj_t t, int l, SDL_Texture *sprt);
    Object(const GameInfo &gInfo, obj_t t, float x, float y, float vx, float vy, SDL_Texture *ss);
    virtual ~Object();
    bool inline isAlive() { return (life > 0); }
    obj_t inline getType() { return type; }
    int inline getLives() { return life; }
    float inline getX() { return _x; };
    float inline getY() { return _y; };
    int inline getW() { return _w; }
    int inline getH() { return _h; }
    float inline getvX() { return _vx; };
    float inline getvY() { return _vy; };
    float inline getaX() { return _ax; };
    float inline getaY() { return _ay; };
    inline Sprite *getCurrSprite() { return curSprite; }
    virtual void update();
    virtual void updatePosition();
    void registerCb(std::function<void(Message *)> cb);
    virtual void hasCollided(obj_t withtype, SDL_Rect overlap_r); // could have passed a special collision struct containing details about collision but its fine for now

protected:
    const GameInfo &gInfo;
    SDL_Texture *sheet;
    obj_t type;
    // bool alive;
    // bool tangible;
    // bool visible;
    int life;
    float _x, _y;   // these will be static_cast<int>()-ed
    float _vx, _vy; //
    float _ax, _ay; //
    // an enum called STATE since I wont be using maps for this purpose
    // not defining here since plain enums have no forward declaration capability
    // typedef enum State{...,..,.,,ANIM_SIZE}State;
    const char *state;
    void changeState(const char *nextstate);
    virtual void updateState();
    // callbacks
    std::function<void(Message *)> dispatchEvent;

    // array of sprites
    std::vector<Sprite *> sprites; // sprites obj ptr s
    // vector<SDL_Rect> collisionBoxes;
    std::map<const char *, Sprite *> spriteset; // associated array for the above
    // map<const char *,SDL_Rect> collisionBoxSet;
    Sprite *curSprite; // ptr to current
    int _w, _h;        // collision shape parameters
    virtual bool initSprites();
    void updateSprite(bool change);
    void updateSpriteFrame();
    void addSprite(const char *name, int n, int maxc, int fps, SDL_Rect base_f, SDL_Texture *img);
    void updateX() { _x = _x + _vx; }
    void updateY() { _y = _y + _vy; }
    void updatevX() { _vx = _vx + _ax; }
    void updatevY() { _vy = _vy + _ay; }
    void setvX(float vx) { _vx = vx; }
    void setvY(float vy) { _vy = vy; }
    void setaX(float ax) { _ax = ax; }
    void setaY(float ay) { _ay = ay; }
    void setY(float y) { _y = y; }
    void setX(float x) { _x = x; }
    // virtual SDL_Rect getNextFrame();
    virtual void checkBoundaryCollision();
    virtual void collisionResponse(obj_t withtype, SDL_Rect overlap_r);
};
#endif