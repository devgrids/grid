#pragma once

namespace grid
{
    enum TextureType : int
    {
        TEXTURE2D_RGB = 0,
        TEXTURE2D_RGBA = 1
    };

    struct __declspec(dllexport) Texture
    {
        ZAR_INLINE Texture();
        ZAR_INLINE ~Texture() = default;
        ZAR_INLINE Texture(const Texture&) = default;
        ZAR_INLINE operator GLuint() const { return id; }
        ZAR_INLINE Texture(const char* path, TextureType type = TextureType::TEXTURE2D_RGB);

        ZAR_INLINE void load(const char* path, TextureType type = TextureType::TEXTURE2D_RGB);
        ZAR_INLINE void bind() const;

        GLint width = 0;
        GLint height = 0;
        GLuint id = 0;
        GLint channels = 0;

        GLuint internal_format = 0;
        GLuint image_format = 0;
        GLuint wrap_s = 0;
        GLuint wrap_t = 0;
        GLuint filter_min = 0;
        GLuint filter_max = 0;

        std::string name;
    };

    using SharedTexture = std::shared_ptr<Texture>;
}
