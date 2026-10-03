#ifndef ZIRCON_UTILS_H
#define ZIRCON_UTILS_H
// RAII all the way
// and use smart pointers instead of this crap
// Unless Memory is Managed and owned by some external library
// like in SDLTextures which are managed by SDL2 itself
#define DELETEOBJ(x)  \
    do                \
    {                 \
        if (x)        \
        {             \
            delete x; \
            x = NULL; \
        }             \
    } while (0);

#endif // ZIRCON_UTILS_H