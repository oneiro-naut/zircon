#ifndef ZIRCON_INPUT_TYPES_H
#define ZIRCON_INPUT_TYPES_H

#include <array>

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
    Count
};

enum class MouseButton
{
    Count
};

template <typename E>
constexpr std::size_t enumClassindex(E e)
{
    return static_cast<std::size_t>(e);
}

struct InputState
{
    std::array<bool, enumClassindex(Key::Count)> keyState;
    std::array<bool, enumClassindex(Key::Count)> systemKeyState;
    std::array<bool, enumClassindex(Key::Count)> mouseButtonState;
};

enum class InputHandlerType
{
    SDL,
    Count
};

#endif // ZIRCON_INPUT_TYPES_H