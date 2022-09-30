#pragma once
#include "../interfaces/interface_system.h"
#include "../objects/game_object.h"

namespace grid
{
    class GameObjectSystem final : public ISystem
    {
    public:
        static GameObjectSystem* instance();
        ~GameObjectSystem() override;
        
        void start() override;
        void update(const float delta_time) override;
        void render() override;

        void add(GameObject* object);
        void add(std::string const& path, bool is_animation = false);
        void remove(GameObject* object);

        void set_projection(glm::mat4 projection);
        void set_view(glm::mat4 view);
        void set_projection_view(glm::mat4 projection, glm::mat4 view);
    private:
        std::vector<GameObject*> objects;
        glm::mat4 projection;
        glm::mat4 view;

        Shader shader_model;
        Shader shader_animation;
    };
}
