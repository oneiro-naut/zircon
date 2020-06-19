#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



int main(int argc,char* argv[])
{
    Window window("Zircon",WIN_H,WIN_W);
    Game game(window);

    while(window.isClosed()==false && game.isOver()== false)
    {
        game.pollEvents();
        game.update();
        if(game.isOver()== false)game.draw();

    }
    
    
    
    return 0;
}