#include "AssetManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

AssetManager::AssetManager(const std::string &path) : m_assetConfigPath(path)
{
    int result = loadAndParseConfig();
    if (result != 0)
    {
        std::cerr << "Invalid config file" << std::endl;
    }
}

AssetManager::~AssetManager()
{
}

int AssetManager::loadAndParseConfig()
{
    std::ifstream file(m_assetConfigPath);

    if (!file)
    {
        std::cerr << "Failed to open file\n";
        return -1;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Asset asset;

        if (!(iss >> asset.id >> asset.role >> asset.path))
        {
            std::cerr << "Invalid line: " << line << '\n';
            continue;
        }

        std::cout << "id   = " << asset.id << '\n';
        std::cout << "role   = " << asset.role << '\n';
        std::cout << "path = " << asset.path << '\n';
        m_assetCtx.assets.push_back(asset);
    }

    return 0;
}