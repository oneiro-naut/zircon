#ifndef ZIRCON_INPUT_HANDLER_H
#define ZIRCON_INPUT_HANDLER_H

#include "InputTypes.h"

#include <memory>

class InputHandlerImpl;

// I wanted to learn PImpl, though turns out here
// Inheritance would be better
// Create this outside Game
// and pass it as a Ref: DI/Composition+Factory
// The same pattern we have used for Renderer
class InputHandler
{
public:
    InputHandler(); // what to pass is not yet known
    ~InputHandler();

    const InputState *getInputState() const;

    void updateInput();

    void pollEvents(std::queue<Event> &eventQ);

private:
    InputState m_inputState; // consolodated input event state structure
    std::unique_ptr<InputHandlerImpl>
        m_impl;
};

#endif // ZIRCON_INPUT_HANDLER_H