#pragma once
#include "object.h"
#include "../data/model.h"
#include "../data/shader.h"
#include "zar/api/opengl/gl_animation.h"
#include "zar/api/opengl/gl_animator.h"

namespace grid
{
    class GameObject : public Object
    {
    public:
        GameObject(std::string const& path, bool is_animation = false);
        void update(const float delta_time) const;
        void render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view) const;
        bool is_animation();
    private:
        Model* model3d{nullptr};
        zar::GLAnimation* animation{nullptr};
        zar::GLAnimator* animator{nullptr};
    };
}
