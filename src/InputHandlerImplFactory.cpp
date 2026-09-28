#include "InputHandlerImplFactory.h"

#include "SDLInputHandler.h"

std::unique_ptr<InputHandlerImpl> InputHandlerImplFactory::create(InputHandlerType type)
{
    if (type == InputHandlerType::SDL)
    {
        return std::make_unique<SDLInputHandler>();
    }
    return nullptr;
}
