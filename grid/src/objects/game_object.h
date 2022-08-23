#pragma once
#include "object.h"
#include "../data/model.h"
#include "../data/Shader.h"
#include "zar/api/opengl/gl_camera.h"

namespace grid
{
    class GameObject : public Object
    {
    public:
        GameObject(std::string const& path, bool gamma = false);
        void render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view) const;

        Model* model3d;
    };
}
