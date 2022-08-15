#include "Texture.h"
#include "stb_image.h"

grid::Texture::Texture():
    width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT),
    filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{
    glGenTextures(1, &this->id);
}

grid::Texture::Texture(const char* path, TextureType type): width(0), height(0), internal_format(GL_RGB),
                                                            image_format(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT),
                                                            filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{
    load(path, type);
}

void grid::Texture::load(const char* path, const TextureType type)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (!data)
    {
        spdlog::error("failed to load texture: {}", path);
        return;
    }

    name = path;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    switch (type)
    {
    case TEXTURE2D_RGB:
        internal_format = GL_RGB;
        image_format = GL_RGB;
        break;
    case TEXTURE2D_RGBA:
        internal_format = GL_RGBA;
        image_format = GL_RGBA;
        break;
    default: ;
    }

    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void grid::Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}

