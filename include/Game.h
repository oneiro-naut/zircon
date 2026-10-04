#ifndef GAME_H
#define GAME_H
#include <vector>
#include <iterator>

#include "Types.h"
#include "Window.h"
#include "Renderer.h"
#include "Object.h"
#include "InputHandler.h"
#include "PhysicsHandler.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "LevelManager.h"
#include "Timer.h"
#include "Rectangle.h"

class Game
{
public:
    Game(Window &window, Renderer &renderer);
    ~Game();
    void pollEvents();
    int run();
    void update();
    void draw();
    void updateScore();
    void genPBullet(float x, float y);
    void genEBullet(float x, float y);
    bool inline isOver() { return m_over; }

private:
    int m_wave;
    int m_score;
    int m_nWaves;

    Renderer &m_renderer;
    TextureManager &m_textureManager;
    Window &m_window;
    std::unique_ptr<InputHandler> m_inputHandler{nullptr};
    std::queue<Event> m_eventQ;
    std::unique_ptr<PhysicsHandler> m_physicsHandler{nullptr};
    AssetManager m_assetManager;

    // Redundant now
    GameInfo m_info;

    ZirconRect m_camera; // just for a simple compile, why? wth

    // Game Objects
    // Currently not Level dependent, ie whatever is running is the Level
    // Needs its own Manager or Layer
    std::unique_ptr<Object> m_player{nullptr};
    std::vector<std::unique_ptr<Object>> m_enemies;
    std::vector<std::unique_ptr<Object>> m_bullets;
    std::vector<std::unique_ptr<Object>> m_ebullets;
    std::vector<std::unique_ptr<Object>> m_powerups;

    // Game Loop States
    bool m_over; // seems redundant to me :)
    enum class State
    {
        Running,
        Paused,
        Over
    };
    State m_state{State::Running};

    // global timer type needed
    Timer m_gameTimer;

    // These need to be loaded via some File listing the assets to be loaded
    // Some Asset Manager needs to be there
    // all sprite-data here /// Can be wrapped into a GameData class,
    Texture m_charsheet;  // tileset1, character texture layer
    Texture m_background; // main texture onto which everything is else rendered

    std::unordered_map<std::string, Texture> m_textureMap;

    void delayFramesPerSecond();

    // Level/Scene initialisation methods
    // Needs lot of work
    // We need a way to load and initialize Objects
    // and have concept of Levels or Maps, etc.
    // Currently everything is hardcoded
    // Input would be: some file
    // It will be read and parsed to create levels/scenes which game would then run
    // LoadLevel fills GameObjects to game, etc
    // Needs the most attention now.
    // global distance traversed (level specific attr)
    // global camera  sdl rect///since our game is side scrolling (level specific)
    void spawnEnemyWave();
    void loadWave();

    // Rendering and Drawing related methods
    bool isInsideScreen(ZirconRect r);
    void drawObjects();
    void drawBackground();

    // Update Game Objects, might destroy dead/inactive objects
    void updatePlayer();
    void updateObjectList(std::vector<std::unique_ptr<Object>> &objList);
    void updateObjects();

    // Physics Layer Collision detection phase
    void updateCollision();

    // Init method, redundant, shift to constructor and add exceptions instead
    // Validatity invariance via ctor will be ensured. RAII enforcement.
    bool initGame();

    // Load Unload Graphical Assets
    bool initTextures();
    void deleteTextures();

    // Game Loop Lifecycle/State related methods
    void endGame();
    void pauseGame();
    void resumeGame();
    void restart();

    // Game Logic, related to Game Loop Life
    // Should be split up to create a Central Logic Layer
    bool checkGameOver();
    inline bool isWaveOver() { return m_enemies.empty(); }
    bool isPlayerDead();

    // Create Game Objects
    // void createBullet
    void genBullet(float x, float y);
    void spawnEnemy();
    void createPlayer(const ObjectCtx &objCtx);

    // Destroy Game Objects
    void freeObjects();
    void deleteObjects();

    // Game Logical Events, Custom Event System managed by Game itself
    // Could arise from non-logical layers, like Physics and Input
    // Ex 1: Press key fires bullet (ie Input triggers Logic)
    // Ex 2: Collision leads to some explosion object creation, or something to spawn etc.
    // (Physics to Logic)
    void onEvent(Message *msg);
    void regObjOnEventCb();
};
#endif