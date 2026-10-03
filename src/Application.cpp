#include "Application.h"
#include "SDLInputHandler.h"
#include "SDLTextureManager.h"
#include "SDLRenderer.h"

#include <iostream>

GameBackend::GameBackend()
{
}

GameBackend::~GameBackend()
{
}

class SDLLib
{
public:
    SDLLib();
    ~SDLLib();
};

class SDLBackend : public GameBackend
{
public:
    SDLBackend();
    virtual ~SDLBackend();

    Renderer &getRenderer() override;
    // TextureManager &getTextureManager() override;
    //  InputHandler &getInputHandler() override;
    Window &getWindow() override;

private:
    // sequence matters alot here
    SDLLib m_sdl;

    Window m_window;

    SDLRenderer m_renderer;
    // SDLInputHandler m_inputHandler;
    // SDLTextureManager m_textureManager;
};

SDLLib::SDLLib()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL." << std::endl;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::cerr << "Failed to initilize SDL_image." << std::endl;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
    }
}

SDLLib::~SDLLib()
{
    // Quit SDL_ttf, img, sdl2
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

SDLBackend::SDLBackend() : m_sdl(), m_window("Game", 1920, 1080),
                           m_renderer(m_window.getWindow(), m_window.getDefaultScreen())
{
}

SDLBackend::~SDLBackend()
{
}

Renderer &SDLBackend::getRenderer()
{
    return m_renderer;
}

Window &SDLBackend::getWindow()
{
    return m_window;
}

std::unique_ptr<GameBackend> createBackend(
    BackendType type)
{
    if (type == BackendType::SDL)
    {
        return std::make_unique<SDLBackend>();
    }
    return nullptr;
}

Application::Application() : m_backend(createBackend(BackendType::SDL)),
                             m_game(m_backend->getWindow(),
                                    m_backend->getRenderer())
{
}

Application::~Application() {}

int Application::run()
{
    return m_game.run();
}
