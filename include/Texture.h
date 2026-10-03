#ifndef ZIRCON_TEXTURE_H
#define ZIRCON_TEXTURE_H

// Texture class, ~ roughly Texture = hwaccelated Image, stored in GPU
// knows about concept of Surfaces (CPU images)
// i am planning not to store any data here, just some generic attributes
// and maybe a handle
struct Texture
{
    int m_handle; // definitely needs a table
    int m_width;
    int m_height;
    // pixel format?
    // etc...
    // is not storing any info of its view
};

#endif // ZIRCON_TEXTURE_H
