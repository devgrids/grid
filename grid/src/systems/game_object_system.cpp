#include "game_object_system.h"

#include "../utility/dev.h"

grid::GameObject_System* grid::GameObject_System::instance()
{
    static GameObject_System instance;
    return &instance;
}

grid::GameObject_System::~GameObject_System()
= default;

void grid::GameObject_System::start()
{
    ISystem::start();
    shader_model = dev::get_shader("model");
    shader_animation = dev::get_shader("animation");
}

void grid::GameObject_System::update(const float& delta_time)
{
    ISystem::update(delta_time);
    for (GameObject*& object : objects)
    {
        object->update(delta_time);
    }
}

void grid::GameObject_System::render()
{
    ISystem::render();
    for (GameObject*& object : objects)
    {
        object->render(
            (object->get_object_type() == ANIMATION)
                ? shader_animation
                : shader_model,
            projection,
            view
        );
    }
}

void grid::GameObject_System::add(GameObject* object)
{
    objects.push_back(object);
}

void grid::GameObject_System::add(std::string const& path, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
                                  const ObjectType& object_type, const PhysicType& physic_type)
{
    objects.push_back(new GameObject(path, position, scale, rotation, object_type, physic_type));
}


void grid::GameObject_System::remove(GameObject* object)
{
}

void grid::GameObject_System::set_projection(const glm::mat4 projection)
{
    this->projection = projection;
}

void grid::GameObject_System::set_view(const glm::mat4 view)
{
    this->view = view;
}

void grid::GameObject_System::set_projection_view(const glm::mat4 projection, const glm::mat4 view)
{
    this->projection = projection;
    this->view = view;
}
