#ifndef ZIRCON_ASSET_MANAGER_H
#define ZIRCON_ASSET_MANAGER_H

#include <string>
#include <vector>

struct Asset
{
    std::string id;
    std::string role; // for only textures so...
    std::string path;
};

struct AssetContext
{
    std::vector<Asset> assets;
};

/*
 - config files
    texture_type texture id(not same as handle) texture file path

 - level files
    object constructor params list

*/
// Tells Game or whatever what textures to load from the texture manager
// and sets their Id, later will be referenced by level loader to refer to associated texture id
//
class AssetManager
{
public:
    AssetManager(const std::string &configPath);
    ~AssetManager();

    const AssetContext &getAssetContext() const { return m_assetCtx; }

private:
    std::string m_assetConfigPath;
    AssetContext m_assetCtx;

    int loadAndParseConfig();
};

#endif // ZIRCON_ASSET_MANAGER_H