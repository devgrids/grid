#pragma once
#include "../interfaces/interface_system.h"
#include "../objects/game_object.h"

namespace grid
{
    class GameObject_System final : public ISystem
    {
    public:
        static GameObject_System* instance();
        ~GameObject_System() override;

        void start() override;
        void update(const float& delta_time) override;
        void render() override;

        void add(GameObject* object);
        void add(std::string const& path, glm::vec3 position = glm::vec3(0.0f),
                 glm::vec3 scale = glm::vec3(1.0f),
                 glm::vec3 rotation = glm::vec3(0.0f),
                 const ObjectType& object_type = MODEL,
                 const PhysicType& physic_type = NON_PHYSICS);
        void remove(GameObject* object);

        void set_projection(glm::mat4 projection);
        void set_view(glm::mat4 view);
        void set_projection_view(glm::mat4 projection, glm::mat4 view);
    private:
        std::vector<GameObject*> objects;
        glm::mat4 projection = {};
        glm::mat4 view = {};

        Shader shader_model;
        Shader shader_animation;
    };
}
