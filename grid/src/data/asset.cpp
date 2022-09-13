#include "asset.h"

#include "vendor/assimp/contrib/stb/stb_image.h"

namespace grid
{
    std::map<std::string, Shader> Asset::shaders;

    Asset::Asset()
    {
        spdlog::info("Static Assets");
    }

    Shader Asset::load_shader(const std::string& name)
    {
        shaders[name] = Shader(name);
        return shaders[name];
    }

    Shader Asset::get_shader(const std::string& name)
    {
        return shaders[name];
    }

    GLuint Asset::load_cubemap(const std::vector<std::string> faces)
    {
        stbi_set_flip_vertically_on_load(false);
        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
        int width, height, nr_components;
        for (GLuint i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nr_components, 0);
            if (data)
            {
                GLenum format;
                if (nr_components == 1)
                    format = GL_RED;
                else if (nr_components == 3)
                    format = GL_RGB;
                else if (nr_components == 4)
                    format = GL_RGBA;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                             data);
                stbi_image_free(data);
            }
            else
            {
                spdlog::error("Cubemap texture failed to load at path: {}", faces[i]);
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return texture_id;
    }

    float Asset::get_aspect_viewport()
    {
        return static_cast<float>(Asset::SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    }

    void Asset::clear()
    {
        for (const auto& iter : shaders)
            glDeleteProgram(iter.second.get_id());
    }
}
