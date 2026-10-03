#ifndef ZIRCON_FONT_MANAGER_H
#define ZIRCON_FONT_MANAGER_H
// TTF_Font *g_font;
// SDL_Surface *message; // text texture layer sits on top of character layer, is it used anywhere?
// ZirconRect status;
// SDL_Color textColor;
// ZirconRect gameend;
// bool loadText();
// void showGameOver(char *winstat);
// void updateStatusText();
#if 0 // All Text/Font related bullshit lives here for now, as dead ass comments :)
void Game::updateStatusText()
{
    char stat[100] = {0};
    sprintf(stat, "Life : %d       Score: %d       Wave : %d", m_player->getLives(), m_score, m_wave);
    // m_renderer.renderText(stat, 0, 0, g_font, textColor);
}

void Game::showGameOver(char *winstat)
{
    // m_renderer.renderText(winstat, m_camera.w / 4, m_camera.h / 4, g_font, textColor);
}

bool Game::loadText()
{
    // Open the font
    // textColor = {255, 255, 255};
    // g_font = TTF_OpenFont("assets/fonts/DejaVuSerif.ttf", 10);
    // return g_font != nullptr;
    return true;
}

init: 
    // message = nullptr;
    // status = createRectangle(0, 0, 500, 30);
    //  gameend = createRectangle(WIN_W / 4, WIN_H / 4, 500, 300);

    // if (!loadText())
    //{
    //     return false;
    // }

destructor:
    // Close the font that was used
    // if (g_font)
    //    TTF_CloseFont(g_font);

#endif

#endif // ZIRCON_FONT_MANAGER_H