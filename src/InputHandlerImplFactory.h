#ifndef ZIRCON_INPUT_HANDLER_FACTORY_H
#define ZIRCON_INPUT_HANDLER_FACTORY_H

#include "InputHandlerImpl.h"
#include <memory>

class InputHandlerImplFactory
{
public:
    static std::unique_ptr<InputHandlerImpl> create(InputHandlerType type);
};

#endif // ZIRCON_INPUT_HANDLER_FACTORY_H