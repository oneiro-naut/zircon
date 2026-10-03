#ifndef ZIRCON_SDL_INPUT_HANDLER_H
#define ZIRCON_SDL_INPUT_HANDLER_H

#include "InputHandlerImpl.h"
#include <SDL2/SDL.h>

class SDLInputHandler : public InputHandlerImpl
{
public:
    SDLInputHandler();
    virtual ~SDLInputHandler();
    void updateInputState(InputState &state) const override;
    void pollEvents(std::queue<Event> &eventQ) override;

private:
    void sdlKeyStateToKeyInputState(InputState &state) const;

private:
    // const int* means --- whatever this points, i wont modify but
    // i can change to what this pointer points to, ie i can change assignments
    // if i wanted to lock this pointer as well i will need uint8* const instead,
    // const to right means ptr or ref itself is const to left it means what it points to will not change
    const Uint8 *m_pSDLKeystate{nullptr}; // keystate array//thru sdl get keystate function
};

#endif // ZIRCON_SDL_INPUT_HANDLER_H