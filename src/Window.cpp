#include "Window.h"
#include <iostream>
//I HATE Static variables 



Window::Window(const string &title,int height,int width): _title(title),_height(height),_width(width)
{
    _closed = false;
    default_screen = { 0, 0, _width, _height}; // will be passed to renderer then Game obj will change it
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
        return false;
    }
    
    renderer = new Renderer(_window, default_screen);
    if(!renderer->initComplete())
    {
        return false;
    }
    cout << "created renderer"<<endl;
    return true;




}

Window::~Window()
{
    delete renderer;
    cout << "detroying window"<<endl;
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

