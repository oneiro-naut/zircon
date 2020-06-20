#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;

#define WIN_W 900
#define WIN_H 700

class Window{
    private:
    std::string _title;
    int _height;
    int _width;
    bool _closed;
    bool initWindow();
    SDL_Window *_window=nullptr;//better than NULL
    public:
        Window(const string &title,int height,int width);
        ~Window();
        bool inline isClosed() const{ return _closed; }
        void pollEvents(SDL_Event event);
        void clear() const;
        int getW();
        int getH();
    private:
        bool init();
        
    public:
    static SDL_Renderer *_renderer;



};
#endif