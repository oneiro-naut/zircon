#include "InputHandler.h"
#include "InputHandlerImplFactory.h"

InputHandler::InputHandler()
{
    m_impl = InputHandlerImplFactory::create(InputHandlerType::SDL);
}

InputHandler::~InputHandler()
{
}

void InputHandler::updateInput()
{
    m_impl->updateInputState(m_inputState);
}

// not sure about the thread safety of this return
const InputState *InputHandler::getInputState() const
{
    return &m_inputState;
}

void InputHandler::pollEvents(std::queue<Event> &eventQ)
{
    m_impl->pollEvents(eventQ);
}
