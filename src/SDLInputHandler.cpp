#include "SDLInputHandler.h"

SDLInputHandler::SDLInputHandler()
{
    m_pSDLKeystate = SDL_GetKeyboardState(nullptr);
    // can this return null?
}

SDLInputHandler::~SDLInputHandler() {}

void SDLInputHandler::updateInputState(InputState &state) const
{
    // take input and convert to desired logical format accepted by the game objects
    // convert sdl event to logical generic event

    // Update internal keystate array
    SDL_PumpEvents();

    sdlKeyStateToKeyInputState(state);
}

void SDLInputHandler::pollEvents(std::queue<Event> &eventQ) // i have a 2KRO keyboard :/
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:

            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            {
                SystemKeyPressedEvent ev{SystemKey::Esc};
                eventQ.push(ev);
            }
            break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_p:
            {
                SystemKeyPressedEvent ev{SystemKey::P};
                eventQ.push(ev);
            }
            break;
            default:
                break;
            }
            break;
        case SDL_QUIT:
        {
            ProcessLevelEvent ev{true};
            eventQ.push(ev);
        }
        break;
        }
    }
    ////////////////////////////cannot press more than 2 normal keys simultaneously unless one of them is a modifier key
    /////////which are wired and programmed to be pressed alongside some other key
    // example if i press LEFT RIGHT and hold them then press UP the UP wont be detected until one of RIGHT or LEFT key is released
    // making space for UP to be detected
    // this is a hardware limitation
    // this is the reason why JUMP should never be DONE using UP key
    // this is the reason why I changed it to LCTRL key which is a modifier
    // nowadays n-key rollover keyboards are there which do not have this (key-ghosting issue)
    // key-ghosting:condition in which beyond a limit key sequences become ambiguous or not detected

    // broke multiple key presses into sequence of key presses
    // every key press changes the state of game
}

void SDLInputHandler::sdlKeyStateToKeyInputState(InputState &state) const
{

    state.keyState[enumClassindex(Key::Down)] = m_pSDLKeystate[SDL_SCANCODE_DOWN];
    state.keyState[enumClassindex(Key::Up)] = m_pSDLKeystate[SDL_SCANCODE_UP];
    state.keyState[enumClassindex(Key::Right)] = m_pSDLKeystate[SDL_SCANCODE_RIGHT];
    state.keyState[enumClassindex(Key::Left)] = m_pSDLKeystate[SDL_SCANCODE_LEFT];
    state.keyState[enumClassindex(Key::Space)] = m_pSDLKeystate[SDL_SCANCODE_SPACE];
}
