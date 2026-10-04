#include "LevelManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

LevelLoader::LevelLoader(const std::string &path) : m_levelFile(path)
{
    int result = loadAndParseObjCtx();
    if (result != 0)
    {
        std::cerr << "Invalid level file" << std::endl;
    }
}

LevelLoader::~LevelLoader() {}

int LevelLoader::loadAndParseObjCtx()
{
    std::ifstream file(m_levelFile);

    if (!file)
    {
        std::cerr << "Failed to open file\n";
        return -1;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        ObjectCtx objCtx;

        if (!(iss >> objCtx.type >> objCtx.life >> objCtx.x >> objCtx.y >> objCtx.vx >> objCtx.vy >> objCtx.textureId))
        {
            std::cerr << "Invalid line: " << line << '\n';
            continue;
        }

        std::cout << "objType   = " << objCtx.type << '\n';
        m_objCtx.objects.push_back(objCtx);
    }

    return 0;
}