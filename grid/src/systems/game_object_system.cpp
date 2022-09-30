#include "game_object_system.h"

#include "../utility/dev.h"

grid::GameObjectSystem* grid::GameObjectSystem::instance()
{
    static GameObjectSystem instance;
    return &instance;
}

grid::GameObjectSystem::~GameObjectSystem()
= default;

void grid::GameObjectSystem::start()
{
    ISystem::start();
    shader_model = dev::get_shader("model");
    shader_animation = dev::get_shader("animation");
}

void grid::GameObjectSystem::update(const float delta_time)
{
    ISystem::update(delta_time);
    for (GameObject*& object : objects)
    {
        object->update(delta_time);
    }
}

void grid::GameObjectSystem::render()
{
    ISystem::render();
    for (GameObject*& object : objects)
    {
        object->render(
            object->is_animation() ? shader_animation : shader_model,
            projection,
            view
        );
    }
}

void grid::GameObjectSystem::add(GameObject* object)
{
    objects.push_back(object);
}

void grid::GameObjectSystem::add(std::string const& path, const bool is_animation)
{
    objects.push_back(new GameObject(path, is_animation));
}

void grid::GameObjectSystem::remove(GameObject* object)
{
}

void grid::GameObjectSystem::set_projection(const glm::mat4 projection)
{
    this->projection = projection;
}

void grid::GameObjectSystem::set_view(const glm::mat4 view)
{
    this->view = view;
}

void grid::GameObjectSystem::set_projection_view(const glm::mat4 projection, const glm::mat4 view)
{
    this->projection = projection;
    this->view = view;
}
