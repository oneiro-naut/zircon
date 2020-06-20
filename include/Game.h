#ifndef GAME_H
#define GAME_H
#include "Window.h"
#include "Object.h"
#include <list>
#include <iterator>

class Object;



class Game{
    public:
    Game(Window &win);
    ~Game();
    SDL_Rect camera;//just for a simple compile 
    void pollEvents();
    void update();
    void draw();
    void updateScore();
    void genPBullet(float x,float y);
    void genEBullet(float x,float y);
    bool inline isOver(){return over;}
    const Uint8* keystate;//keystate array//thru sdl get keystate function
    uint32_t timer[TIMER_TYPE_SIZE];
    private:
    int wave;
    int score;
    int n_waves;
    Object* player;
    list<Object*> enemies;
    list<Object*> pbullets;
    list<Object*> ebullets;
    list<Object*> powerups;
    bool over;
    
    enum State{RUNNING,PAUSED,OVER}state=RUNNING;
    Window &pwindow;
    //global timer type needed
    Uint32 game_timer;
    //global distance traversed
    //global camera  sdl rect///since our game is side scrolling
    
    //all sprite-data here
    TTF_Font * g_font;
    SDL_Surface *message ;
    SDL_Rect status ;
    SDL_Color textColor;
    SDL_Rect gameend;
    SDL_Texture* charsheet;//tileset1
    //SDL_Texture*
    //SDL_Texture* overworld;//tileset2
    //SDL_Texture* gameFrame;
    void delayFramesPerSecond();
    void spawnEnemyWave();
    void loadWave();
    void loadWave1();
    bool loadText();
    void showGameOver(char * winstat);

    void apply_text(SDL_Surface* surface,SDL_Rect position);
    void loadWave1Enemies();
    //void transform();
    void updatePlayer();
    void updateStatusText();
    bool isInsideScreen(SDL_Rect r);
    void updateEnemies();
    void updateBullets();
    void updatePBullets();
    void updateEBullets();
    void updatePowerups();
    //void updateCamera();
    SDL_Rect createRectangle(int x,int y,int w,int h);
    void updateCollision();
    void applyRender();// get rect overlap of all entities with camera and apply them to rendering target
    bool initGame();
    bool initTextures();
    void renderText(char* text,SDL_Rect position);
    void applyTexture(Object* obj,float scale);
    void applyTextureEx(Object* obj, float scale, double angle, SDL_Point * center, SDL_RendererFlip flip);
    SDL_Texture* createTexture(string path);
    SDL_Texture* loadTexture(const char * image,SDL_Surface* surface);
    int inline getRectArea(SDL_Rect r){ return r.w * r.h;};
    SDL_Rect getOverlapRect(SDL_Rect r1,SDL_Rect r2);
    void endGame();
    void pauseGame();
    void resumeGame();
    void createPlayer();
    //void createBullet
    void genBullet(float x,float y);
    void spawnEnemy();
    bool isWaveOver();
    bool isEnemiesEmpty();
    bool isPlayerDead();
    void restart();
    void deleteTextures();
    void freeObjects();
    SDL_Rect positionObjFrame(Object* obj,float scale);
    void deleteEnemy(list <Object*> :: iterator it );
    void deleteBullet(list <Object*> :: iterator it );  // We need something like these functions as well
    void deletePowerup(list <Object*> :: iterator it );
    void checkCollision(Object* obj1,Object* obj2);
};
#endif