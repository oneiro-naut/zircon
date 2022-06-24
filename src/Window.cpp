#include "Window.h"
#include <iostream>
// I HATE Static variables

Window::Window(const std::string &title, int height, int width) : _title(title), _height(height), _width(width)
{
    _closed = false;
    default_screen = {0, 0, _width, _height}; // will be passed to renderer then Game obj will change it
    if (!initWindow())
    {
        _closed = true;
    }
}

bool Window::initWindow()
{

    _window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, 0); // c_str converts cpp string to c string
    std::cout << "Created window" << std::endl;
    if (_window == nullptr)
    {
        std::cerr << "Failed to create window." << std::endl;
        return false;
    }

    return true;
}

Window::~Window()
{
    std::cout << "detroying window" << std::endl;
    SDL_DestroyWindow(_window);
}

void Window::pollEvents(SDL_Event event)
{

    switch (event.type)
    {
    case SDL_QUIT:
        _closed = true;
        std::cout << "Closing window!" << std::endl;
        break;
    case SDL_MOUSEMOTION:
        // std::cout<<event.motion.x<<","<<event.motion.y<<std::endl;
        break;
    case SDL_MOUSEBUTTONDOWN:
        // std::cout<<"Mouse clicked!"<<std::endl;
        break;
    case SDL_MOUSEBUTTONUP:
        // std::cout<<"Mouse botton released!"<<std::endl;
        break;
    default:
        break;
    }
}

int Window::getW()
{
    return _width;
}

int Window::getH()
{
    return _height;
}
