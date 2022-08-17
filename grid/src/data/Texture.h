#pragma once
#include "zar/api/opengl/gl_texture.h"

namespace grid
{
    class __declspec(dllexport) Texture final : public zar::GLTexture
    {
    public:
        Texture();
        Texture(const char* path);
        ~Texture();

        void load(const char* path) override;
        std::string type;
    };

    using SharedTexture = std::shared_ptr<Texture>;
}
