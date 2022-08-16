#include "Texture.h"
#include "stb_image.h"

grid::Texture::Texture():
    GLTexture()
{

}

grid::Texture::Texture(const char* path): GLTexture(path)
{
    Texture::load(path);
}

grid::Texture::~Texture()
= default;

void grid::Texture::load(const char* path)
{
    GLTexture::load(path);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (!data)
    {
        spdlog::error("failed to load texture: {}", path);
        return;
    }
    GLTexture::generate(path, data);
    stbi_image_free(data);
}


