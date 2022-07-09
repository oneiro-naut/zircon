#ifndef ZIRCON_UTILS_H
#define ZIRCON_UTILS_H

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