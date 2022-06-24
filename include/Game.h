#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iterator>

#include "Types.h"
#include "Window.h"
#include "Renderer.h"
#include "Object.h"

class Game
{
public:
    Game();
    ~Game();
    SDL_Rect camera; // just for a simple compile
    void pollEvents();
    int run();
    void update();
    void draw();
    void updateScore();
    void genPBullet(float x, float y);
    void genEBullet(float x, float y);
    bool inline isOver() { return over; }
    const Uint8 *keystate; // keystate array//thru sdl get keystate function
    uint32_t timer[TIMER_TYPE_SIZE];
    Renderer *renderer;
    Window *window;

private:
    int wave;
    int score;
    int n_waves;
    GameInfo info;
    Object *player;

    std::vector<Object *> enemies;
    std::vector<Object *> pbullets;
    std::vector<Object *> ebullets;
    std::vector<Object *> powerups;
    bool over;

    enum State
    {
        RUNNING,
        PAUSED,
        OVER
    } state = RUNNING;

    // global timer type needed
    Uint32 game_timer;
    // global distance traversed
    // global camera  sdl rect///since our game is side scrolling

    // all sprite-data here /// Can be wrapped into a GameData class, can be made sdl2 independent by more creating more wrappers
    TTF_Font *g_font;
    SDL_Surface *message;
    SDL_Rect status;
    SDL_Color textColor;
    SDL_Rect gameend;
    SDL_Texture *charsheet; // tileset1
    SDL_Texture *background;
    // SDL_Texture*
    // SDL_Texture* overworld;//tileset2
    // SDL_Texture* gameFrame;
    void delayFramesPerSecond();
    void spawnEnemyWave();
    void loadWave();
    void drawObjects();
    void loadWave1();
    bool loadText();
    void showGameOver(char *winstat);
    void drawBackground();
    void loadWave1Enemies();
    // void transform();
    void updatePlayer();
    void updateStatusText();
    bool isInsideScreen(SDL_Rect r);
    void updateEnemies();
    void updateBullets();
    void updatePBullets();
    void updateEBullets();
    void updatePowerups();
    // void updateCamera();

    void updateCollision();
    bool initGame();
    bool initTextures();
    SDL_Texture *createTexture(std::string path); // we need a texture class as well
    SDL_Texture *loadTexture(const char *image, SDL_Surface *surface);

    void endGame();
    void pauseGame();
    void resumeGame();
    void createPlayer();
    // void createBullet
    void genBullet(float x, float y);
    void spawnEnemy();
    bool isWaveOver();
    bool isEnemiesEmpty();
    bool isPlayerDead();
    void restart();
    void deleteTextures();
    void freeObjects();
    SDL_Rect positionObjFrame(Object *o, float scale);
    void deleteEnemy(std::vector<Object *>::iterator it);
    void deleteBullet(std::vector<Object *>::iterator it); // We need something like these functions as well
    void deletePowerup(std::vector<Object *>::iterator it);
    void checkCollision(Object *obj1, Object *obj2); // in future a collision class as well which can do complex stuff
    void onEvent(Message *msg);
    bool SDLInit();
    void regObjOnEventCb();
};
#endif