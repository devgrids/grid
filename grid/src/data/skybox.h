#pragma once
#include "Shader.h"
#include "zar/api/opengl/gl_camera.h"

namespace grid
{
    class GLSkybox
    {
    public:
        GLSkybox(const std::string& name, const std::string& type);
        void render(const zar::GLCamera& camera, glm::vec3 ambient) const;
        void load_skybox(const std::string& name, const std::string& type);
        void config_cube_map();
        void config_shader();
    private:
        std::string name_skybox;

        Shader shader_cubemap;
        Shader shader_skybox;

        GLuint cubemap_texture{};
        GLuint cube_vao{}, cube_vbo{};
        GLuint skybox_vao{}, skybox_vbo{};
    };

}

