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

void SDLInputHandler::sdlKeyStateToKeyInputState(InputState &state) const
{

    state.keyState[enumClassindex(Key::Down)] = m_pSDLKeystate[SDL_SCANCODE_DOWN];
    state.keyState[enumClassindex(Key::Up)] = m_pSDLKeystate[SDL_SCANCODE_UP];
    state.keyState[enumClassindex(Key::Right)] = m_pSDLKeystate[SDL_SCANCODE_RIGHT];
    state.keyState[enumClassindex(Key::Left)] = m_pSDLKeystate[SDL_SCANCODE_LEFT];
    state.keyState[enumClassindex(Key::Space)] = m_pSDLKeystate[SDL_SCANCODE_SPACE];
}
