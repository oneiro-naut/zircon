#ifndef ZIRCON_INPUT_HANDLER_H
#define ZIRCON_INPUT_HANDLER_H

#include "InputTypes.h"

#include <memory>

class InputHandlerImpl;

class InputHandler
{
public:
    InputHandler(); // what to pass is not yet known
    ~InputHandler();

    const InputState *getInputState() const;

    void updateInput();
    // void pollEvents(); // updates keystate array val receieved from sdl, also "polls"
    // void initKeyState(); [register sdl2 events or whatever library we would use] // can be done in the constructor as well
private:
    InputState m_inputState; // consolodated input event state structure
    std::unique_ptr<InputHandlerImpl>
        m_impl;
    //[sdl2] const Uint8 *keystate; // keystate array//thru sdl get keystate function
};

#endif // ZIRCON_INPUT_HANDLER_H