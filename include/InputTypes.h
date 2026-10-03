#ifndef ZIRCON_INPUT_TYPES_H
#define ZIRCON_INPUT_TYPES_H

#include <array>
#include <queue>
#include <variant>

// More Event Types can be added here

enum class Key
{
    Up,
    Down,
    Left,
    Right,
    Space,
    Count
};

enum class SystemKey
{
    Esc,
    P,
    Count
};

enum class MouseButton
{
    Left,
    Right,
    Count
};

struct ProcessLevelEvent
{
    bool closed; // not sure what else could happen
};

struct SystemKeyPressedEvent
{
    SystemKey key;
};

struct SystemKeyReleasedEvent
{
    SystemKey key;
};

struct MouseMotionEvent
{
};

struct MouseButtonPressedEvent
{
    MouseButton btn;
};

struct MouseButtonReleasedEvent
{
    MouseButton btn;
};

using Event = std::variant<
    ProcessLevelEvent,
    SystemKeyPressedEvent>;

template <typename E>
constexpr std::size_t enumClassindex(E e)
{
    return static_cast<std::size_t>(e);
}

struct InputState
{
    std::array<bool, enumClassindex(Key::Count)> keyState;
};

enum class InputHandlerType
{
    SDL,
    Count
};

#endif // ZIRCON_INPUT_TYPES_H