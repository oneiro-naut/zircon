#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <SDL2/SDL.h>

#define WIN_W 1920
#define WIN_H 1080

class Window
{
private:
    std::string _title;
    int _height;
    int _width;
    bool _closed;
    bool initWindow();
    SDL_Window *_window = nullptr; // better than NULL
    SDL_Rect default_screen;       // default screen to everything
public:
    Window(const std::string &title, int height, int width);
    ~Window();
    inline bool isClosed() const { return _closed; }
    void pollEvents(SDL_Event event);
    int getW();
    int getH();
    inline SDL_Window *getWindow() { return _window; };
    inline SDL_Rect getDefaultScreen() { return default_screen; }

private:
    bool init();
};
#endif