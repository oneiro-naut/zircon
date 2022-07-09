#include <iostream>
#include <cstdio>
#include <string.h>

#include "Game.h"
#include "Util.h"
#include "Renderer.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Rectangle.h"
//#include "Powerup.h"

Game::Game()
{
    // player=new Player(*this,10,10,50,50,0,255,0,255,0,0,0,0);
    over = false;
    charsheet = nullptr;
    background = nullptr;
    g_font = nullptr;
    renderer = nullptr;

    if (!initGame())
    {
        over = true;
    }

    if (!over)
        std::cout << "Created Game" << std::endl;
}

Game::~Game()
{
    deleteObjects();
    deleteTextures();

    std::cout << "Destroying Game" << std::endl;
    // Close the font that was used
    if (g_font)
        TTF_CloseFont(g_font);

    // Quit SDL_ttf, img, sdl2
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::updateScore()
{
    score += 5;
    std::cout << "Current Score = " << score << std::endl;
}

void Game::showGameOver(char *winstat)
{
    renderer->renderText(winstat, camera.w / 4, camera.h / 4, g_font, textColor);
}

void Game::updateStatusText()
{
    char stat[100] = {0};
    sprintf(stat, "Life : %d       Score: %d       Wave : %d", player->getLives(), score, wave);
    renderer->renderText(stat, 0, 0, g_font, textColor);
}

void Game::updateEnemies()
{
    std::vector<Object *>::iterator it;
    for (it = enemies.begin(); it != enemies.end(); it++)
    {
        if (!(*it)->isAlive())
        {
            DELETEOBJ(*it)
            // std::cout<<"deleted 1 bullet"<<std::endl;
            it = enemies.erase(it); // returns next valid it
            if (it == enemies.end())
            {
                // std::cout<<"reached end"<<std::endl;
                break;
            }
            // it++; // would it save us? no its undefined behaviour
        }
        (*it)->update();
    }
}

void Game::update()
{
    if (over)
        return;
    delayFramesPerSecond(); // so delay should be here not in draw/render functions
    // if (state == OVER)
    // {
    //     return;
    // }
    game_timer = SDL_GetTicks();

    // //update-player
    updatePlayer();
    // //update-enemies
    updateEnemies();
    // //update-bullets
    updateBullets();
    // //update-powerups
    // //updatePowerups();
    // //problem is how do we update collision without the position rect
    // //maybe with a checkCollision(obj1,obj2) function;
    updateCollision();
    updateStatusText();
}

void Game::deleteObjects()
{
    DELETEOBJ(player)
    std::vector<Object *>::iterator it;
    for (it = pbullets.begin(); it != pbullets.end();)
    {
        DELETEOBJ(*it)
        it = pbullets.erase(it);
    }
    for (it = ebullets.begin(); it != ebullets.end();)
    {
        DELETEOBJ(*it)
        it = ebullets.erase(it);
    }
    for (it = enemies.begin(); it != enemies.end();)
    {
        DELETEOBJ(*it)
        it = enemies.erase(it);
    }
}

void Game::endGame()
{
    over = true;
}

void Game::updatePlayer()
{
    if (player)
        player->update();
}

void Game::updateBullets()
{
    updatePBullets();
    updateEBullets();
}

void Game::updatePBullets()
{
    std::vector<Object *>::iterator it;
    for (it = pbullets.begin(); it != pbullets.end(); it++)
    {
        if (!(*it)->isAlive())
        {
            DELETEOBJ(*it)
            // std::cout<<"deleted 1 bullet"<<std::endl;
            it = pbullets.erase(it); // returns next valid it
            if (it == pbullets.end())
            {
                // std::cout<<"reached end"<<std::endl;
                break;
            }
            // it++; // would it save us? no its undefined behaviour
        }
        (*it)->update();
    }
}

void Game::updateEBullets()
{
    std::vector<Object *>::iterator it;
    for (it = ebullets.begin(); it != ebullets.end(); it++)
    {
        if (!(*it)->isAlive())
        {
            DELETEOBJ(*it)
            // std::cout<<"deleted 1 bullet"<<std::endl;
            it = ebullets.erase(it); // returns next valid it
            if (it == ebullets.end())
            {
                // std::cout<<"reached end"<<std::endl;
                break;
            }
            // it++; // would it save us? no its undefined behaviour
        }
        (*it)->update();
    }
}

void Game::genPBullet(float x, float y)
{
    Object *bullet = new Bullet(info, PBULLET, x, y, 6, 0, charsheet);
    using namespace std::placeholders;
    bullet->registerCb(std::bind(&Game::onEvent, this, _1));
    pbullets.push_back(bullet);
}

void Game::genEBullet(float x, float y)
{
    Object *bullet = new Bullet(info, EBULLET, x, y, -6, 0, charsheet);
    using namespace std::placeholders;
    bullet->registerCb(std::bind(&Game::onEvent, this, _1));
    ebullets.push_back(bullet);
}

SDL_Rect Game::positionObjFrame(Object *o, float scale)
{
    SDL_Rect rect = {
        static_cast<int>(o->getX()),
        static_cast<int>(o->getY()),
        static_cast<int>(o->getW() * scale),
        static_cast<int>(o->getH() * scale)};

    return rect;
}

void Game::checkCollision(Object *obj1, Object *obj2)
{
    // more details in copy
    // we can use dynamic programming here n memoiz some stuff or maybe not
    SDL_Rect r1 = positionObjFrame(obj1, 1);
    SDL_Rect r2 = positionObjFrame(obj2, 1); // scale = 1

    SDL_Rect inter = getOverlapRect(r1, r2);
    int area = 0;
    area = getRectArea(inter);

    if (area == 0)
        return; // no collision
    else if (area > 0)
    { // collision occured
        // notify the pair
        obj1->hasCollided(obj2->getType(), inter);
        obj2->hasCollided(obj1->getType(), inter);
    } // we r cool now
}

void Game::updateCollision()
{
    // use for loops with iterators to iterate thru pairs
    std::vector<Object *>::iterator it;
    std::vector<Object *>::iterator iter;
    // player-enemy pairs
    for (it = enemies.begin(); it != enemies.end(); it++)
    {
        checkCollision(player, *it);
    }
    // player-ebullet pairs
    for (it = ebullets.begin(); it != ebullets.end(); it++)
    {
        checkCollision(player, *it);
    }
    // enemy-enemy pairs
    for (it = enemies.begin(); it != enemies.end(); it++)
    {
        for (iter = enemies.begin(); iter != enemies.end(); iter++)
        {
            if (it != iter)
            {
                checkCollision(*iter, *it);
            }
        }
    }
    // player-powerup pairs
    for (it = powerups.begin(); it != powerups.end(); it++)
    {
        checkCollision(player, *it);
    }
    // enemy-pbullet pairs
    for (it = enemies.begin(); it != enemies.end(); it++)
    {
        for (iter = pbullets.begin(); iter != pbullets.end(); iter++)
        {

            checkCollision(*iter, *it);
        }
    }
    // pbullet-ebullet pairs
    for (it = pbullets.begin(); it != pbullets.end(); it++)
    {
        for (iter = ebullets.begin(); iter != ebullets.end(); iter++)
        {

            checkCollision(*iter, *it);
        }
    }

    // sweet ;)
}

bool Game::checkGameOver()
{
    char win[50] = "You Win!";
    char lose[50] = "You lose!";

    if (enemies.empty())
    {
        endGame();
        std::cout << "You win!" << std::endl;
        std::cout << "Game Over!" << std::endl;
        showGameOver(win);
    }
    // testobj->update();
    if (!player->isAlive())
    {
        endGame();
        std::cout << "You lose!" << std::endl;
        std::cout << "Game Over!" << std::endl;
        showGameOver(lose);
    }
    return over;
}

void Game::pollEvents() // i have a 2KRO keyboard :/
{
    SDL_Event event;
    if (checkGameOver())
        return;
    SDL_PumpEvents(); // update keystate array

    if (SDL_PollEvent(&event) && keystate)
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:

            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                over = true;
                std::cout << "Closing Game!" << std::endl;
                over = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_p:
                if (state == RUNNING)
                {
                    state = PAUSED;
                    std::cout << "Game paused" << std::endl;
                    break;
                }
                else if (state == PAUSED)
                {
                    state = RUNNING; // resume
                    std::cout << "Game Running" << std::endl;
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
    // example if i press LEFT RIGHT and hold them then press UP the UP wont be detected until one of RIGHT or LEFT key is released
    // making space for UP to be detected
    // this is a hardware limitation
    // this is the reason why JUMP should never be DONE using UP key
    // this is the reason why I changed it to LCTRL key which is a modifier
    // nowadays n-key rollover keyboards are there which do not have this (key-ghosting issue)
    // key-ghosting:condition in which beyond a limit key sequences become ambiguous or not detected

    // broke multiple key presses into sequence of key presses
    // every key press changes the state of game

    window->pollEvents(event);
}

void Game::delayFramesPerSecond()
{
    if ((SDL_GetTicks() - game_timer) < (1000 / 30))
    {
        SDL_Delay((1000 / 30) - (SDL_GetTicks() - game_timer));
    }
}

void Game::drawBackground()
{
    renderer->renderTexture(background, camera, 0, 0, 1);
}

void Game::draw()
{
    if (over == false)
    {
        drawBackground();
        drawObjects();
    }
    renderer->clear();

    if (over == true)
    {
        SDL_Delay(3000);
    }
}

void Game::loadWave()
{
    spawnEnemyWave();
}

void Game::spawnEnemyWave()
{
    if (enemies.empty() && wave == 0) // init wave 0 test wave
    {
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 50, 50, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 50, 100, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 90, 50, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 90, 72, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 130, 50, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 130, 350, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 170, 600, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 170, 400, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 200, 200, charsheet));
        enemies.push_back(new Enemy(info, ENEMY, 1, camera.w + 200, 500, charsheet));
        // loadWave1Enemies();
    }
    for (auto &e : enemies)
    {
        using namespace std::placeholders;
        e->registerCb(std::bind(&Game::onEvent, this, _1));
    }
}

void Game::createPlayer()
{
    player = new Player(info, PLAYER, 3, 32, 32, charsheet);
    using namespace std::placeholders;
    player->registerCb(std::bind(&Game::onEvent, this, _1));
}

bool Game::initGame()
{
    wave = 0;
    n_waves = 1;
    game_timer = SDL_GetTicks(); // global timer initialization
    message = nullptr;
    status = createRectangle(0, 0, 500, 30);
    gameend = createRectangle(WIN_W / 4, WIN_H / 4, 500, 300);
    state = RUNNING;
    timer[GLOBAL_TIMER] = SDL_GetTicks();
    camera = createRectangle(0, 32, WIN_W, WIN_H - 32);
    if (!SDLInit())
    {
        return false;
    }
    keystate = SDL_GetKeyboardState(NULL);
    info.sceneHeight = camera.h;
    info.sceneWidth = camera.w;
    info.keystate = keystate;

    window = new Window("Zircon", WIN_H, WIN_W);
    if (!window->getWindow())
    {
        return false;
    }
    renderer = new Renderer(window->getWindow(), window->getDefaultScreen());
    if (!renderer->initComplete())
    {
        return false;
    }
    std::cout << "Created renderer" << std::endl;
    renderer->changeScreen(camera);

    if (!initTextures())
    {
        return false;
    }
    if (!loadText())
    {
        return false;
    }
    // testobj = new Object(*this,PLAYER,1,30,30,charsheet);
    createPlayer();
    loadWave();
    std::cout << "Player created" << std::endl;
    return true;
}

bool Game::initTextures()
{
    charsheet = createTexture("assets/newsprtsheet.png");
    background = createTexture("assets/background.png");
    return charsheet && background;
}

void Game::drawObjects() // now this is the real mess
{
    // render player
    renderer->renderSprite(player->getCurrSprite(), player->getX(), player->getY());
    // render enemies
    for (auto &e : enemies)
        renderer->renderSprite(e->getCurrSprite(), e->getX(), e->getY());
    // render bullets
    for (auto &pb : pbullets)
        renderer->renderSprite(pb->getCurrSprite(), pb->getX(), pb->getY());
    for (auto &eb : ebullets)
        renderer->renderSprite(eb->getCurrSprite(), eb->getX(), eb->getY());
}

bool Game::loadText()
{
    // Open the font
    textColor = {255, 255, 255};
    g_font = TTF_OpenFont("assets/fonts/DejaVuSerif.ttf", 10);
    return g_font != nullptr;
}

SDL_Texture *Game::loadTexture(const char *image, SDL_Surface *surface)
{
    SDL_Texture *texture = NULL;

    if (surface == NULL)
    {
        if (image == NULL)
        {
            fprintf(stderr, "[%s: %d]Warning: image string NULL\n", __FILE__, __LINE__);
            return NULL;
        }

        surface = IMG_Load(image);

        if (surface == NULL)
        {
            fprintf(stderr, "[%s: %d]Warning: Could not load image %s into surface, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
            return NULL;
        }
    }

    texture = renderer->createTexturefromSurface(surface);

    if (texture == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: Could not create texture %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture *Game::createTexture(std::string path)
{

    const char *image = path.c_str();
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;

    surface = IMG_Load(image);

    if (surface == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: Could not load image %s into surface, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return NULL;
    }

    if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x0, 0x0, 0x0)))
    {
        fprintf(stderr, "[%s: %d]Warning: Could not set color key for image %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
        return NULL;
    }

    texture = loadTexture(image, surface);

    if (texture == NULL)
    {
        fprintf(stderr, "[%s: %d]Warning: Could not create textureBack %s, error: %s\n", __FILE__, __LINE__, image, SDL_GetError());
    }

    return texture;
}

void Game::deleteTextures()
{
    SDL_DestroyTexture(charsheet);
}

int Game::run()
{
    while (!isOver() && !window->isClosed())
    {
        pollEvents();
        update();
        draw();
    }
    return 0;
}

bool Game::SDLInit()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL." << std::endl;
        return false; // means it failed
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        std::cerr << "Failed to initilize SDL_image." << std::endl;
        return false;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        return false;
    }

    return true;
}

void Game::onEvent(Message *msg)
{
    // switch case to call specific Handler
    switch (msg->code)
    {
    case 0: // visitor pattern can be used instead of dynamic_casting, we are adding new member vars...eqv to adding new getter methods..not very polymorphic..not relying on virtual methods
            // EnemyUpdateMessage *emsg = dynamic_cast<EnemyUpdateMessage *>(msg);
    {
        EFireBulletMessage *emsg = static_cast<EFireBulletMessage *>(msg); // static_cast isnt safe here but its fast :/ we r assuming obj are derived
        genEBullet(emsg->x - emsg->w, emsg->y + (emsg->h / 4));
    }
    break;
    case 1:
    {
        EnemyCollidedMessage *ec = static_cast<EnemyCollidedMessage *>(msg);
        if (ec->withObj == obj_t::PBULLET)
            updateScore();
    }
    break;
    case 2:
        break;
    case 3:
    {
        PFireBulletMessage *pmsg = static_cast<PFireBulletMessage *>(msg);
        genPBullet(pmsg->x + pmsg->w - 8, pmsg->y + (pmsg->h / 4));
    }
    break;
    default:
        break;
    }
}
