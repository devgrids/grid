#pragma once
#include <PxRigidDynamic.h>

#include "object.h"
#include "../data/model.h"
#include "../data/shader.h"
#include "zar/api/opengl/gl_animation.h"
#include "zar/api/opengl/gl_animator.h"

namespace grid
{
    enum ObjectType { MODEL, ANIMATION };

    enum PhysicType { NON_PHYSICS, SPHERE };

    class GameObject : public Object
    {
    public:
        explicit GameObject(
            std::string const& path,
            glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 scale = glm::vec3(1.0f),
            glm::vec3 rotation = glm::vec3(0.0f),
            const ObjectType& object_type = MODEL,
            const PhysicType& physic_type = NON_PHYSICS
        );
        void update(const float delta_time);
        void render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view) const;
        ObjectType get_object_type() const;
        PhysicType get_physic_type() const;
    private:
        Model* model3d{nullptr};
        zar::GLAnimation* animation{nullptr};
        zar::GLAnimator* animator{nullptr};
        physx::PxRigidDynamic* body{nullptr};

        ObjectType object_type;
        PhysicType physic_type;
    };
}
