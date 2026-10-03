#include "Timer.h"

#include <SDL/SDL.h>

class SDLTimer
{
public:
    SDLTimer(int timeout);
    ~SDLTimer();
    bool updateTickOnTimeout();
    bool resetTickOnTimeout();
    void delayFPS(int fps);

    void updateTick();

    void delayMS(int ms);

    void reset();

    inline void setTimeout(int timeout) { m_timeout = timeout; }

private:
    Uint32 m_timer{0};
    Uint32 m_timeout{0};
};

SDLTimer::SDLTimer(int timeout) : m_timeout{static_cast<Uint32>(timeout)}
{
    // m_timer = SDL_GetTicks();
    m_timer = 0; // basically first call to updateontimeout always returns true this way
}

SDLTimer::~SDLTimer()
{
}

void SDLTimer::updateTick()
{
    m_timer = SDL_GetTicks();
}

bool SDLTimer::updateTickOnTimeout()
{
    if (m_timer == 0 || SDL_GetTicks() > m_timer)
    {
        m_timer = SDL_GetTicks() + m_timeout;
        return true;
    }
    return false;
}

bool SDLTimer::resetTickOnTimeout()
{
    if (m_timer == 0) // start timer, within timeout
    {
        m_timer = SDL_GetTicks() + m_timeout;
        return false;
    }
    else if (SDL_GetTicks() < m_timer) // already started timer, within timeout
    {
        return false;
    }
    m_timer = 0; // reset on time out
    return true; // timed out
}

void SDLTimer::delayFPS(int fps)
{
    if ((SDL_GetTicks() - m_timer) < (1000 / fps))
    {
        SDL_Delay((1000 / fps) - (SDL_GetTicks() - m_timer));
    }
}

void SDLTimer::delayMS(int ms)
{
    SDL_Delay(static_cast<Uint32>(ms));
}

void SDLTimer::reset()
{
    m_timer = 0;
}

Timer::Timer(int timeout)
{
    m_impl = std::make_unique<SDLTimer>(timeout);
}

Timer::Timer()
{
    m_impl = std::make_unique<SDLTimer>(1000);
}

Timer::~Timer()
{
}

bool Timer::updateTickOnTimeout()
{
    return m_impl->updateTickOnTimeout();
}

bool Timer::resetTickOnTimeout()
{
    return m_impl->resetTickOnTimeout();
}

void Timer::delayFPS(int fps)
{
    m_impl->delayFPS(fps);
}

void Timer::delayMS(int ms)
{
    m_impl->delayMS(ms);
}

void Timer::updateTick()
{
    m_impl->updateTick();
}

void Timer::reset()
{
    m_impl->reset();
}

void Timer::setTimeout(int timeout)
{
    m_impl->setTimeout(timeout);
}
