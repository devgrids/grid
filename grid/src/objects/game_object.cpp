#include "game_object.h"

#include "../data/application.h"

grid::GameObject::GameObject(std::string const& path, bool gamma)
{
    model3d = new Model(path, gamma);
}

void grid::GameObject::render(const Shader& shader,const glm::mat4 projection,const glm::mat4 view) const
{
    shader.use();
    
    shader.set_mat4("projection", projection);
    shader.set_mat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set_mat4("model", model);

    model3d->draw(shader);
}
