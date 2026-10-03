#ifndef ZIRCON_TIMER_H
#define ZIRCON_TIMER_H

#include <memory>

class SDLTimer;

// unit = milliseconds for everything here :)
class Timer
{
public:
    Timer(int timeout);
    Timer();
    ~Timer();

    bool updateTickOnTimeout(); // returns true if tick was updated, ie 1 or more timeout have elapsed since last call
    bool resetTickOnTimeout();  // returns true if timed out and resets the tick, otherwise either starts timer or is already started timer
    void delayFPS(int fps);
    void updateTick();
    void delayMS(int ms);
    void reset();

    void setTimeout(int timeout);

private:
    std::unique_ptr<SDLTimer> m_impl{nullptr};
};

#endif // ZIRCON_TIMER_H
