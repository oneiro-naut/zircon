#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Powerup.h"
#include <iostream>
#include <stdio.h>
#include <string.h>



Game::Game(Window &pwin):pwindow(pwin)
{

    //player=new Player(*this,10,10,50,50,0,255,0,255,0,0,0,0);
    over = false;

    if(!initGame())
    {
        over = true;
    }
    keystate = SDL_GetKeyboardState(NULL);
    
    cout<<"Created Game"<<endl;
}

Game::~Game()
{
    deleteTextures();

    cout<<"Destroying Game"<<endl;
        //Close the font that was used
    TTF_CloseFont( g_font );
    
    //Quit SDL_ttf
    TTF_Quit();
}

void Game::updateScore(){
    score +=5;
    cout << "Current Score = "<<score<<endl;
}


void Game::showGameOver(char * winstat)
{
    renderText(winstat,gameend);
}

void Game::updateStatusText()
{
    char stat[100];
    sprintf(stat,"Life : %d       Score: %d       Wave : %d",player->getLives(),score,wave);
    renderText(stat,status);

}



void Game::updateEnemies()
{
    list <Object*> :: iterator it;
    for(it = enemies.begin();it!= enemies.end();it++)
    {
        if(!(*it)->isAlive())
        {
            delete (*it);
            //cout<<"deleted 1 bullet"<<endl;
            it = enemies.erase(it);//returns next valid it
            if(it == enemies.end())
            {
                //cout<<"reached end"<<endl;
                break;
            }
            //it++; // would it save us? no its undefined behaviour
        }
        (*it)->update();
    }

}

void Game::update()
{
    if(state == OVER)
    {

        return;
    }
    game_timer = SDL_GetTicks();
    char win[50]= "You Win!";
    char lose[50] = "You lose!";
    
    if(isEnemiesEmpty())
    {
        endGame();
        cout<<"You win!"<<endl;
        cout<<"Game Over!"<<endl;
        //char * win = "You Win!";
        showGameOver(win);

        return;
    }

    if(isPlayerDead())
    {
        endGame();
        cout<<"You lose!"<<endl;
        cout<<"Game Over!"<<endl;
        //char * lose = "You Lose!";
        showGameOver(lose);

        return;
    }

    //update-player
    updatePlayer();
    //update-enemies
    updateEnemies();
    //update-bullets
    updateBullets();
    //update-powerups
    //updatePowerups();
    //problem is how do we update collision without the position rect
    //maybe with a checkCollision(obj1,obj2) function;
    updateCollision();
    updateStatusText();
   
}

bool Game::isPlayerDead()
{
    return !(player->isAlive());
}
void Game::endGame()
{
    over = true;
    //state = OVER;
    delete player;
    list <Object*> :: iterator it ;
    for(it = pbullets.begin();it !=pbullets.end();it++)
    {
        delete *it;
        it = pbullets.erase(it);
    }

    for(it = ebullets.begin();it !=ebullets.end();it++)
    {
        delete *it;
        it = ebullets.erase(it);
    }
    for(it = enemies.begin();it !=enemies.end();it++)
    {
        delete *it;
        it = enemies.erase(it);
    }
    
}
void Game::updatePlayer()
{

    player->update();
}

void Game::updateBullets()
{
    updatePBullets();
    updateEBullets();
}

void Game::updatePBullets()
{
    list <Object*> :: iterator it;
    for(it = pbullets.begin();it!= pbullets.end();it++)
    {
        if(!(*it)->isAlive())
        {
            delete (*it);
            //cout<<"deleted 1 bullet"<<endl;
            it = pbullets.erase(it);//returns next valid it
            if(it == pbullets.end())
            {
                //cout<<"reached end"<<endl;
                break;
            }
            //it++; // would it save us? no its undefined behaviour
        }
        (*it)->update();
    }
}


void Game::updateEBullets()
{
    list <Object*> :: iterator it;
    for(it = ebullets.begin();it!= ebullets.end();it++)
    {
        if(!(*it)->isAlive())
        {
            delete (*it);
            //cout<<"deleted 1 bullet"<<endl;
            it = ebullets.erase(it);//returns next valid it
            if(it == ebullets.end())
            {
                //cout<<"reached end"<<endl;
                break;
            }
            //it++; // would it save us? no its undefined behaviour
        }
        (*it)->update();
    }
}

bool Game::loadText()
{
       //Open the font
    textColor = { 255, 255, 255};
    g_font = TTF_OpenFont( "../assets/fonts/DejaVuSerif.ttf", 10 );
    if(g_font!=nullptr)return true;
    else return false;
}

void Game::genPBullet(float x,float y)
{
    Object* bullet = new Bullet(*this,PBULLET,x,y,8,0,charsheet);
    pbullets.push_back(bullet);
}

void Game::genEBullet(float x,float y)
{
    Object* bullet = new Bullet(*this,EBULLET,x,y,-5,0,charsheet);
    ebullets.push_back(bullet);
}


bool Game::isWaveOver()
{
    return isEnemiesEmpty();
}

bool Game::isEnemiesEmpty()
{
    if(enemies.size()==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}




void Game::checkCollision(Object* obj1,Object* obj2)
{
    //more details in copy
    //we can use dynamic programming here n memoiz some stuff or maybe not
    SDL_Rect r1 = positionObjFrame(obj1,1);
    SDL_Rect r2 = positionObjFrame(obj2,1);//scale = 1 

    SDL_Rect inter = getOverlapRect(r1,r2);
    int area = 0;
    area = getRectArea(inter);

    if(area==0)return; // no collision
    else if(area > 0){ //collision occured
        //notify the pair
        obj1->hasCollided(obj2->getType(),inter);
        obj2->hasCollided(obj1->getType(),inter);
    }//we r cool now

}

void Game::updateCollision()
{
    //use for loops with iterators to iterate thru pairs
    list <Object*> :: iterator it;
    list <Object*> :: iterator iter;
    //player-enemy pairs
    for( it = enemies.begin();it != enemies.end();it++)
    {
        checkCollision(player,*it);
    }
    //player-ebullet pairs
    for( it = ebullets.begin();it != ebullets.end();it++)
    {
        checkCollision(player,*it);
    }    
    //enemy-enemy pairs
    for( it = enemies.begin();it != enemies.end();it++)
    {
        for(iter = enemies.begin();iter != enemies.end();iter++)
        {
            if(it != iter)
            {
                checkCollision(*iter,*it);
            }
        }
    }
    //player-powerup pairs
    for( it = powerups.begin();it != powerups.end();it++)
    {
        checkCollision(player,*it);
    }    
    //enemy-pbullet pairs
    for( it = enemies.begin();it != enemies.end();it++)
    {
        for(iter = pbullets.begin();iter != pbullets.end();iter++)
        {
            
            
            checkCollision(*iter,*it);
            
        }
    }
    //pbullet-ebullet pairs
    for( it = pbullets.begin();it != pbullets.end();it++)
    {
        for(iter = ebullets.begin();iter != ebullets.end();iter++)
        {
            
            
            checkCollision(*iter,*it);
            
        }
    }


    //sweet ;)
}


void Game::deleteTextures()
{
    SDL_DestroyTexture(charsheet);
}

void Game::pollEvents()//i have a 2KRO keyboard :/
{
    SDL_Event event;
    SDL_PumpEvents();//update keystate array


    if(SDL_PollEvent(&event)&&keystate){

    switch(event.type)
    {
        case SDL_KEYDOWN:
        
        switch(event.key.keysym.sym){
            case SDLK_ESCAPE:
            over = true;
            cout<<"Closing Game!"<<endl;
            state = OVER;
            over = true;
            break;
            
        }
        break;
        case SDL_KEYUP:
        switch(event.key.keysym.sym)
        {
            case SDLK_p:
            if(state == RUNNING)
            {
                state = PAUSED;
                cout<<"Game paused"<<endl;
                break;
            }
            else if(state == PAUSED)
            {
                state = RUNNING;//resume
                cout<<"Game Running"<<endl;
                break;
            }
            break;
            default:
            break;
        }
        break;
    }


    
    }

////////////////////////////cannot press more than 2 normal keys simultaneously unless one of them is a modifier key 
/////////which are wired and programmed to be pressed alongside some other key
//example if i press LEFT RIGHT and hold them then press UP the UP wont be detected until one of RIGHT or LEFT key is released 
//making space for UP to be detected
//this is a hardware limitation
//this is the reason why JUMP should never be DONE using UP key
//this is the reason why I changed it to LCTRL key which is a modifier 
//nowadays n-key rollover keyboards are there which do not have this (key-ghosting issue)
//key-ghosting:condition in which beyond a limit key sequences become ambiguous or not detected 



            //broke multiple key presses into sequence of key presses 
            //every key press changes the state of game
        
        pwindow.pollEvents(event);
       
}

void Game::delayFramesPerSecond()
{
    if((SDL_GetTicks() - game_timer) < (1000 / 30))
    {
        SDL_Delay((1000 / 30) - (SDL_GetTicks() - game_timer));
    }    
}


void Game::draw()                                                                                                                                                                                                       
{
  
  if(over== false)applyRender();
  pwindow.clear();
  delayFramesPerSecond();  
  if(over==true && state != OVER)
  {
      SDL_Delay(3000);
      state = OVER;
  }
}

void Game::loadWave()
{
    spawnEnemyWave();
}

void Game::spawnEnemyWave()
{
    Object* e = nullptr;
    list <Object*> :: iterator it ;
    if(isEnemiesEmpty() && wave == 0)//init wave 0 test wave
    {
        enemies.push_back(new Enemy(*this,1,camera.w+50,50,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+50,100,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+90,50,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+90,72,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+130,50,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+130,350,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+170,600,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+170,400,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+200,200,charsheet));
        enemies.push_back(new Enemy(*this,1,camera.w+200,500,charsheet));
        
            
        
        //loadWave1Enemies();
    }
    //
}

void Game::createPlayer()
{
    player = new Player(*this,3,charsheet);
}


void Game::apply_text(SDL_Surface* surface,SDL_Rect position)
{
    SDL_Texture * texture = nullptr;
    

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0));

    texture = SDL_CreateTextureFromSurface(Window::_renderer, surface);

  
    SDL_RenderCopy(Window::_renderer,texture,NULL,&position);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}

void Game::renderText(char* text,SDL_Rect position)
{
    message = TTF_RenderText_Solid( g_font,text, textColor );
    apply_text(message, position );

}


bool Game::initGame()
{
    wave = 0;
    n_waves = 1;
    game_timer = SDL_GetTicks();//global timer initialization
    message = nullptr;
    status = createRectangle(0,0,500,30);
    gameend = createRectangle(WIN_W/4,WIN_H/4,500,300);
    state = RUNNING;
    timer[GLOBAL_TIMER] = SDL_GetTicks();
    camera = createRectangle(0,32,WIN_W,WIN_H-32);
    if(!initTextures())
    {
        return false;
    }
    if(!loadText())
    {
        return false;
    }
    createPlayer();
    loadWave(); 
    cout<<"Player created"<<endl;
    return true;
}

SDL_Texture * Game::loadTexture(const char * image, SDL_Surface * surface)
{
    SDL_Texture * texture = NULL;

    if(surface == NULL)
    {
        if(image == NULL)
        {
            fprintf(stderr, "[%s: %d]Warning: image string NULL\n", __FILE__, __LINE__);
            return NULL;

        }

        if(strstr(image, "bmp") == NULL)
        {
            fprintf(stderr, "[%s: %d]Warning: Images are not bitmap\n", __FILE__, __LINE__);
            return NULL;
        }

        surface = IMG_Load(image);

        if(surface == NULL)
        {
            fprintf(stderr, "[%s: %d]Warning: Could not load image %s into surface, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
            return NULL;
        }



    }

    texture = SDL_CreateTextureFromSurface(Window::_renderer, surface);

    if(texture == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: Could not create texture %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;

}

SDL_Texture* Game::createTexture(string path)
{

    const char* image = path.c_str();
    SDL_Texture * texture = NULL;
    SDL_Surface * surface = NULL;

    surface = IMG_Load(image);

    if(surface == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: Could not load image %s into surface, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return NULL;
    }

    if(SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0)))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not set color key for image %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return NULL;
    }

    texture = loadTexture(image, surface);

    if(texture == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: Could not create textureBack %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
    }

    return texture;


}

SDL_Rect Game::createRectangle(int x,int y,int w,int h)
{
    SDL_Rect r = {0,0,0,0};
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    
    return r;
}

bool Game::initTextures()
{
    charsheet = createTexture("../assets/sprite-sheet.bmp");
    if(charsheet!=nullptr)return true;
    else return false;
}

bool Game::isInsideScreen(SDL_Rect r)
{
    SDL_Rect overlap = getOverlapRect(r,camera);
    if(getRectArea(overlap)!=0)return true;
    else return false;
}

void Game::applyTextureEx(Object* obj,float scale, double angle, SDL_Point * center, SDL_RendererFlip flip)
{
    SDL_Rect offset = positionObjFrame(obj,scale);

    if(!isInsideScreen(offset))
    {
        return ;
    }

    SDL_Rect cur = obj->getCurrentSprite();
    if(SDL_RenderCopyEx(Window::_renderer, obj->sheet,&cur, &offset, angle, center, flip))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not render copy, error: %s\n",__FILE__, __LINE__, SDL_GetError());
    }
}
void Game::applyTexture(Object* obj,float scale)
{
    applyTextureEx(obj,scale, 0, NULL, SDL_FLIP_NONE);
}

SDL_Rect Game::getOverlapRect(SDL_Rect r1,SDL_Rect r2)//found it physics n-body-simulation
{
    int x0=r1.x;
    int y0=r1.y;
    int w0=r1.w;
    int h0=r1.h;
    int x1=r2.x;
    int y1=r2.y;
    int w1=r2.w;
    int h1=r2.h;
    
    int interx1 = 0;
    int interx2 = 0;
    int intery1 = 0;
    int intery2 = 0;

    if(x0<x1&&x1<x0+w0){ interx1 = x1;}
    if(x0<x1+w1&&x1+w1<x0+w0){interx2 = x1+w1;}
    if(x1<x0&&x0<x1+w1){interx1 = x0;}
    if(x1<x0+w0&&x0+w0<x1+w1){interx2 = x0+ w0;}
    if(x0==x1&&w0==w1 ){interx1 = x0; interx2 = x0+w0;}
    if(y0<y1&&y1<y0+h0){ intery1 = y1;}
    if(y0<y1+h1&&y1+h1<y0+h0){intery2 = y0+h0;}
    if(y1<y0&&y0<(y1+h1)){intery1 = y0;}
    if(y1<(y0+h0)&&y0+h0<(y1+h1)){intery2 = y0+h0;}
    if( y0==y1&&h0==h1){intery1 = y0; intery2 = y0+h0;}
    
    SDL_Rect interrect ={interx1,intery1,interx2-interx1,intery2-intery1};
    return interrect;
}

SDL_Rect Game::positionObjFrame(Object* obj,float scale)
{   //relying on implicit type casting NO
    SDL_Rect objrect = {
        static_cast<int>(obj->getX()),
        static_cast<int>(obj->getY()),
        obj->getW(),
        obj->getH()
    };
    objrect.w *= scale;
    objrect.h *= scale;
    return objrect;
}

void Game::applyRender()
{
    list <Object*> :: iterator it;
    //render player
    applyTexture(player,1);
    //render enemies
    for(it = enemies.begin();it != enemies.end();it++)
    {
        applyTexture(*it,1);
    }
    //render bullets
    for(it = pbullets.begin();it != pbullets.end();it++)
    {
        applyTexture(*it,1);
    }
    for(it = ebullets.begin();it != ebullets.end();it++)
    {
        applyTexture(*it,1);
    }    
    //render powerups
    for(it = powerups.begin();it != powerups.end();it++)
    {
        applyTexture(*it,1);
    }    

}