#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string.h>

#include "Util.h"
#include "Renderer.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
//#include "Powerup.h"
#include "Game.h"

Game::Game(Window &window, Renderer &renderer) : m_window(window),
                                                 m_renderer(renderer),
                                                 m_textureManager(m_renderer.getTextureManager())
{
    m_over = false;

    m_inputHandler = std::make_unique<InputHandler>();
    m_physicsHandler = std::make_unique<PhysicsHandler>();

    if (!initGame())
    {
        m_over = true;
    }

    if (!m_over)
        std::cout << "Created Game" << std::endl;
}

Game::~Game()
{
    deleteObjects();
    deleteTextures();

    std::cout << "Destroying Game" << std::endl;
}

void Game::updateScore()
{
    m_score += 5;
    std::cout << "Current Score = " << m_score << std::endl;
}

void Game::updateObjectList(std::vector<std::unique_ptr<Object>> &objList)
{
    for (auto &obj : objList)
    {
        obj->update();
    }

    objList.erase(
        std::remove_if(
            objList.begin(),
            objList.end(),
            [](const auto &obj)
            {
                return !obj->isAlive();
            }),
        objList.end());
}

void Game::update()
{
    if (m_over || m_state == State::Paused)
        return;
    delayFramesPerSecond(); // so delay should be here not in draw/render functions

    m_gameTimer.updateTick();

    m_physicsHandler->updatePhysics(*m_player,
                                    m_enemies,
                                    m_bullets,
                                    m_ebullets,
                                    m_powerups,
                                    m_info);
    updateObjects();

    // //problem is how do we update collision without the position rect
    // //maybe with a checkCollision(obj1,obj2) function;
    updateCollision(); // not your problem anymore discuss this inside Physics Handler :)
    // updateStatusText();
}

void Game::updateObjects()
{
    // //update-player
    updatePlayer();
    // //update-enemies
    updateObjectList(m_enemies);
    // //update-bullets
    updateObjectList(m_bullets);
    updateObjectList(m_ebullets);
    // //update-powerups
    // //updatePowerups(); wtf is a powerup?
}

void Game::deleteObjects()
{
    m_enemies.clear();
    m_bullets.clear();
    m_ebullets.clear();
    // no need to delete m_player manually
}

void Game::endGame()
{
    m_over = true;
}

void Game::updatePlayer()
{
    if (m_player)
        m_player->update();
}

void Game::genPBullet(float x, float y)
{
    auto bullet = std::make_unique<Bullet>(PBULLET, x, y, 10, 0, m_charsheet);

    bullet->registerCb(
        std::bind(&Game::onEvent, this, std::placeholders::_1));

    m_bullets.push_back(std::move(bullet));
}

void Game::genEBullet(float x, float y)
{
    auto bullet = std::make_unique<Bullet>(EBULLET, x, y, -10, 0, m_charsheet);

    bullet->registerCb(
        std::bind(&Game::onEvent, this, std::placeholders::_1));

    m_ebullets.push_back(std::move(bullet));
}

void Game::updateCollision()
{
    m_physicsHandler->handleCollisions(*m_player, m_enemies,
                                       m_bullets, m_ebullets, m_powerups);
    // sweet ;)
}

bool Game::checkGameOver()
{
    char win[50] = "You Win!";
    char lose[50] = "You lose!";

    if (m_enemies.empty())
    {
        endGame();
        std::cout << "You win!" << std::endl;
        std::cout << "Game Over!" << std::endl;
        // showGameOver(win);
    }

    if (!m_player->isAlive())
    {
        endGame();
        std::cout << "You lose!" << std::endl;
        std::cout << "Game Over!" << std::endl;
        // showGameOver(lose);
    }
    return m_over;
}

void Game::pollEvents()
{
    m_inputHandler->updateInput(); // :) // now only updates keyboaard input state which was shared with the Object types

    if (checkGameOver())
        return;

    m_inputHandler->pollEvents(m_eventQ);

    // Event Dispatcher single threaded, naive impl
    // not really a dispatch just event handler is called
    // probably what dispatch means tho :)
    bool skipDuplicatePauseEvent = false; // one pause event per frame/game loop iteration :)
    // current logic might cause dropped events
    while (!m_eventQ.empty())
    {
        // const auto &ev = m_eventQ.front();
        std::visit([this, &skipDuplicatePauseEvent](const auto &event)
                   {
            using T = std::decay_t<decltype(event)>;

            if constexpr (std::is_same_v<T, ProcessLevelEvent>)
            {
                // Window object handles these type of events
                m_window.handleEvent(event);
            }
            else if constexpr (std::is_same_v<T, SystemKeyPressedEvent>)
            {
                if (event.key == SystemKey::Esc)
                {
                    m_over = true;
                    std::cout << "Closing Game!" << std::endl;
                }
                else if (event.key == SystemKey::P && !skipDuplicatePauseEvent)
                {
                    if (m_state == State::Running)
                    {
                        m_state = State::Paused;
                        std::cout << "Game paused" << std::endl;
                    }
                    else if (m_state == State::Paused)
                    {
                        m_state = State::Running; // resume
                        std::cout << "Game Running" << std::endl;
                    }
                    skipDuplicatePauseEvent = true;
                }
            } },
                   m_eventQ.front());
        m_eventQ.pop();
    }
}

void Game::delayFramesPerSecond()
{
    m_gameTimer.delayFPS(30);
}

void Game::drawBackground()
{
    // Experimented with scrolling, find a better way to incorporate this idea :)
    static int scrollX = 0;
    if (scrollX < m_camera.w)
    {
        scrollX += 1;
    }
    else
    {
        scrollX = 0; // or whatever the start is
    }
    // render 1
    m_renderer.renderTexture(m_background, m_camera, scrollX, 0, 1);
    // render 2 ... remaining portion
    m_renderer.renderTexture(m_background, m_camera, -1 * m_camera.w + scrollX, 0, 1);
}

void Game::draw()
{
    if (m_over == false)
    {
        drawBackground();
        drawObjects();
    }
    m_renderer.clear();

    if (m_over == true)
    {
        m_gameTimer.delayMS(3000);
    }
}

void Game::loadWave()
{
    spawnEnemyWave();
}

void Game::spawnEnemyWave()
{
    if (m_enemies.empty() && m_wave == 0) // init wave 0 test wave
    {
        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 50, 50, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 50, 228, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 90, 356, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 90, 72, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 130, 50, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 130, 484, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 170, 728, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 170, 400, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 200, 200, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 200, 500, m_charsheet));
        // loadWave1Enemies();

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 400, 50, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 500, 228, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 600, 356, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 900, 72, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 800, 50, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 1000, 484, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 1170, 728, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 1170, 400, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 2000, 200, m_charsheet));

        m_enemies.push_back(
            std::make_unique<Enemy>(ENEMY, 1, m_camera.w + 2100, 500, m_charsheet));
        // loadWave1Enemies();
    }

    for (auto &e : m_enemies)
    {
        using namespace std::placeholders;
        e->registerCb(std::bind(&Game::onEvent, this, _1));
    }
}

void Game::createPlayer()
{
    m_player = std::make_unique<Player>(0, m_info.sceneHeight / 2, PLAYER, 3, 32 * 2, 32 * 2, m_charsheet);
    using namespace std::placeholders;
    m_player->registerCb(std::bind(&Game::onEvent, this, _1));
}

bool Game::initGame()
{
    m_wave = 0;
    m_nWaves = 1;
    m_gameTimer.updateTick();
    m_state = State::Running;
    m_camera = createRectangle(0, 32, WIN_W, WIN_H - 32);

    Object::registerInput(m_inputHandler->getInputState());
    m_info.sceneHeight = m_camera.h;
    m_info.sceneWidth = m_camera.w;

    if (!m_window.getWindow())
    {
        return false;
    }
    if (!m_renderer.initComplete())
    {
        return false;
    }
    std::cout << "Created renderer" << std::endl;
    m_renderer.changeScreen(m_camera);

    if (!initTextures())
    {
        return false;
    }

    createPlayer();
    loadWave();
    std::cout << "Player created" << std::endl;
    return true;
}

bool Game::initTextures()
{
    m_charsheet = m_textureManager.loadTexture("assets/newsprtsheet.png");
    m_background = m_textureManager.loadTexture("assets/background.png");
    return m_charsheet.m_handle >= 0 && m_background.m_handle >= 0;
}

void Game::deleteTextures()
{
    m_textureManager.unloadTexture(m_charsheet.m_handle);
    m_textureManager.unloadTexture(m_charsheet.m_handle);
}

void Game::drawObjects() // now this is the real mess
{
    // render player
    m_renderer.renderSprite(m_player->getCurrSprite(), m_player->getX(), m_player->getY());
    // render enemies
    for (auto &e : m_enemies)
        m_renderer.renderSprite(e->getCurrSprite(), e->getX(), e->getY());
    // render bullets
    for (auto &pb : m_bullets)
        m_renderer.renderSprite(pb->getCurrSprite(), pb->getX(), pb->getY());
    for (auto &eb : m_ebullets)
        m_renderer.renderSprite(eb->getCurrSprite(), eb->getX(), eb->getY());
}

int Game::run()
{
    while (!isOver() && !m_window.isClosed())
    {
        pollEvents();
        update();
        draw();
    }
    return 0;
}

// smells like shit, improve this, but this is smart shit
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
