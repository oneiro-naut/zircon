#ifndef ZIRCON_SPRITE_MANAGER_H
#define ZIRCON_SPRITE_MANAGER_H

#include <unordered_map>

// Manager that maintains sprite Ids and relation with actual underlying texture handles
// Want to create to decouple underlying Texture from the consumers
// A better way to represent Textures would be via Handles
// Same goes for individual Sprite group in them, grouped via sprite Ids and coords
// This manager assumes old school sprite sheet format
// ie each sprite group will contain its entire animation in single row
// each animation sprite will be same width and height as well
// currently subblocks are not supported
// each renderable entity would entirely be represented by a single block of w, h
// which can of course be scaled, etc, by integer amount
class SpriteManager
{
public:
private: // TODO
};

#endif // ZIRCON_SPRITE_MANAGER_H
