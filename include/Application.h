#ifndef ZIRCON_APPLICATION_H
#define ZIRCON_APPLICATION_H

#include "Game.h"
#include "Window.h"

// Could have been called factory or whatever and this will be aware of all the fucking concrete stuff

/*
- Compose stuff
- Choose backends

Game Needs:
Graphics Group: External
- Renderer
- Asset/TextureManager

I/O Group: External
- InputHandler

GUI/Windowing: External
- WindowHandler

GUIGraphicsIO group (combines all): External
- Renderer
- Asset/TextureManager
- InputHandler
- WindowHandler


PhysicsGroup: Internal // maybe later on for now this can be completely internal

GameObjects: Internal

*/

enum class BackendType // various pairs can exist
{
    SDL,
    Count
};

class GameBackend
{
public:
    GameBackend();
    virtual ~GameBackend();

    virtual Renderer &getRenderer() = 0;
    // virtual InputHandler &getInputHandler() = 0; // TODO
    virtual Window &getWindow() = 0;

    // Renderer m_renderer;
    // InputHandler m_inputHandler;
    // TextureManager m_textureManager;
};

// for factory always use unique pointers not ref, here ref cant be used
// use ref for compositions
std::unique_ptr<GameBackend> createBackend(
    BackendType type);

class Application
{
public:
    Application();
    ~Application();
    int run();

private:
    std::unique_ptr<GameBackend> m_backend{nullptr};

    Game m_game;
};

#endif // ZIRCON_APPLICATION_H
