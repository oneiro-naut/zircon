#include "Window.h"
#include <iostream>


SDL_Renderer *Window::_renderer = nullptr;

Window::Window(const string &title,int height,int width): _title(title),_height(height),_width(width)
{
    _closed = false;
    if(!initWindow())
    {
        _closed = true;
    }
}


bool Window::initWindow()
{
    if(SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        cerr << "Failed to initialize SDL." << endl;
        return false;//means it failed 
    }
    
    if(IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG)
    {
        cerr<<"Failed to initilize SDL_image."<<endl;
        return false;
    }
       //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;    
    }
    _window = SDL_CreateWindow(_title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,_width,_height,0);//c_str converts cpp string to c string
    cout<<"Created window"<<endl;
    if(_window==nullptr)
    {
        cerr << "Failed to create window." <<endl;
        return 0;
    }
    

    _renderer= SDL_CreateRenderer(_window,-1,SDL_RENDERER_ACCELERATED);

    if(_renderer==nullptr)
    {
        cerr << "Failed to create renderer" <<endl;
        return 0;
    
    }

    return true;




}

Window::~Window()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    IMG_Quit();
    SDL_Quit();
}

void Window::pollEvents(SDL_Event event)
{
    
        switch(event.type)
        {
            case SDL_QUIT:
                _closed = true;
                cout<<"Closing window!"<<endl;
                break;

            case SDL_MOUSEMOTION:
                //cout<<event.motion.x<<","<<event.motion.y<<endl;
                break;
            case SDL_MOUSEBUTTONDOWN:
                //cout<<"Mouse clicked!"<<endl;    
                break;
            case SDL_MOUSEBUTTONUP:
                //cout<<"Mouse botton released!"<<endl;
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

void Window::clear() const
{
    SDL_RenderPresent(_renderer);
    SDL_SetRenderDrawColor(_renderer,0,0,0,255);
    SDL_RenderClear(_renderer);
    
}

