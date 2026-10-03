#ifndef ZIRCON_INPUT_HANDLER_IMPL_H
#define ZIRCON_INPUT_HANDLER_IMPL_H

#include "InputTypes.h"

class InputHandlerImpl
{
public:
    InputHandlerImpl();
    virtual ~InputHandlerImpl();
    virtual void updateInputState(InputState &state) const = 0;
    virtual void pollEvents(std::queue<Event> &eventQ) = 0;
};

#endif // ZIRCON_INPUT_HANDLER_IMPL_H