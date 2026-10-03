#include "Sprite.h"
#include <iostream>
using namespace std;

Sprite::Sprite(int n, int c, int fps,
               ZirconRect base,
               Texture img) : m_totalFrames(n), m_maxLoopCount(c), m_fps(fps),
                              m_baseFrame(base), m_img(img)
{
    m_over = false;
    m_currFrame = m_baseFrame;
    m_clock.setTimeout(1000 / m_fps);
    m_clock.reset();
}

Sprite::~Sprite()
{
}

void Sprite::update()
{
    if (m_clock.updateTickOnTimeout())
    {
        updateFrame(); // may or may not reset the CLOCK
    }
}

void Sprite::stop()
{
    reset();
}

void Sprite::reset()
{
    m_currFrame = m_baseFrame;
    m_frameCount = 1;
    m_loopCount = 0;
    m_clock.reset();
    m_over = false;
}

void Sprite::updateFrame()
{
    if (m_over == true)
        return;

    if (m_frameCount >= m_totalFrames)
    {
        m_loopCount++;
        if (m_maxLoopCount == -1)
            m_frameCount = 1; // cylic
        else if (m_maxLoopCount > 0)
        {
            if (m_loopCount >= m_maxLoopCount)
            {
                m_over = true;
                reset();
            }
            m_frameCount = 1;
        }
        m_currFrame = m_baseFrame;
    }
    else
    {
        m_frameCount++;
        m_currFrame.x += m_currFrame.w;
    }
}

ZirconRect Sprite::getCurrentFrame()
{
    return m_currFrame;
}
