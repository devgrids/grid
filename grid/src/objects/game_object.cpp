#include "game_object.h"

grid::GameObject::GameObject(std::string const& path, const bool is_animation)
{
    model3d = new Model(path, false);
    if (is_animation)
    {
        animation = new zar::GLAnimation(
            path,
            model3d->get_bone_info_map(),
            model3d->get_bone_count()
        );
        animator = new zar::GLAnimator(animation);
    }
}

void grid::GameObject::update(const float delta_time) const
{
    if (animator != nullptr)
    {
        animator->update_animation(delta_time);
    }
}

void grid::GameObject::render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view) const
{
    shader.use();

    shader.set_mat4("projection", projection);
    shader.set_mat4("view", view);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);

    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    
    transform = glm::scale(transform, scale);
    shader.set_mat4("model", transform);

    if (animator != nullptr)
    {
        const auto transforms = animator->get_final_bone_matrices();
        for (int i = 0; i < transforms.size(); ++i)
            shader.set_mat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
    }
    model3d->draw(shader);
}

bool grid::GameObject::is_animation()
{
    return animation;
}
