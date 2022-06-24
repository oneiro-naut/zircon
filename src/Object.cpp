#include "Object.h"
#include <iostream>
using namespace std;
// having 3 constructors n only 1 of them having renderer init caused undefined behaviour therefore make only 1 base contructor for
// every entity to avoid further ambuities

Object::Object(const GameInfo &gInfo, obj_t t, int l, SDL_Texture *sprt) : gInfo(gInfo), type(t), life(l), sheet(sprt)
{
}

Object::Object(const GameInfo &gInfo, obj_t t, float x, float y, float vx, float vy, SDL_Texture *ss)
    : gInfo(gInfo), type(t), _x(x), _y(y), _vx(vx), _vy(vy), sheet(ss)
{
}

Object::Object(const GameInfo &gInfo, obj_t t, int l, int w, int h, SDL_Texture *ss) : gInfo(gInfo), type(t), life(l), _w(w), _h(h), sheet(ss)
{
    _x = 200;
    _y = 200;
    _vx = 0;
    _vy = 0;
    _ax = 0;
    _ay = 0;
    if (!initSprites())
    {
        life = 0;
    }
}

Object::~Object()
{
}

void Object::updateSprite(bool change)
{
    if (change == true)
    {
        curSprite->stop();
        curSprite = spriteset[state];
    }

    // curSprite->update();
}

void Object::updateSpriteFrame()
{
    curSprite->update();
}

void Object::update()
{
    updateSpriteFrame();
    updateState();
}

void Object::updatePosition()
{
    updateX();
    updateY();
    checkBoundaryCollision();
}

void Object::collisionResponse(obj_t withtype, SDL_Rect overlap_r)
{
}

void Object::checkBoundaryCollision()
{
}

void Object::updateState()
{
    if (curSprite->isOver())
    {
        if (!strcmp(state, "shield"))
            changeState("idle");
    }
}

void Object::changeState(const char *nextstate)
{
    bool changed = true;
    if (!strcmp(nextstate, state))
    {
        changed = false;
    }
    state = nextstate;
    updateSprite(changed);
}

void Object::addSprite(const char *name, int n, int maxc, int fps, SDL_Rect base_f, SDL_Texture *img)
{
    Sprite *s = new Sprite(n, maxc, fps, base_f, img);
    sprites.push_back(s);
    spriteset.emplace(name, s); // made name value pair for the sprite
}

bool Object::initSprites() // can be parsed from a file
{
    //     SDL_Rect idle = {192,32,32,32};//can store these offsets in a file to parse
    // //    SDL_Rect sh = {320,32,32,32};
    //     //sdl rect up sdl rect down...
    //     state = "idle";
    //     addSprite(state,4,-1,10,idle,sheet);
    //     curSprite = spriteset[state];
    return true;
}

void Object::hasCollided(obj_t withtype, SDL_Rect overlap_r)
{
    cout << "Collision occured!" << endl;
}

void Object::registerCb(std::function<void(Message *)> cb)
{
    dispatchEvent = cb;
}