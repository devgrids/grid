#include "game_object.h"

#include "../systems/physics_system.h"

grid::GameObject::GameObject(std::string const& path, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation,
                             const ObjectType& object_type, const PhysicType& physic_type)
{
    this->object_type = object_type;
    this->physic_type = physic_type;

    this->position = position;
    this->scale = scale;
    this->rotation = rotation;

    model3d = new Model(path, false);
    if (object_type == ANIMATION)
    {
        animation = new zar::GLAnimation(
            path,
            model3d->get_bone_info_map(),
            model3d->get_bone_count()
        );
        animator = new zar::GLAnimator(animation);
    }

    if (physic_type == SPHERE)
    {
        body = Physics_System::instance()->create_dynamic(
            physx::PxTransform(physx::PxVec3(0, 20, 0)),
            physx::PxSphereGeometry(10), physx::PxVec3(0, -50, -1)
        );
    }
}

void grid::GameObject::update(const float delta_time)
{
    if (animator != nullptr)
    {
        animator->update_animation(delta_time);
    }

    if (physic_type == SPHERE)
    {
        position = glm::vec3(body->getGlobalPose().p.x,
                             body->getGlobalPose().p.y - 10.0f,
                             body->getGlobalPose().p.z);

        rotation = glm::vec3(body->getGlobalPose().q.x,
                             body->getGlobalPose().q.y,
                             body->getGlobalPose().q.z);

        // spdlog::info("angle: {}", body->getGlobalPose().q.getAngle(body->getGlobalPose().q));

        // spdlog::info("rotation ({},{},{})", body->getGlobalPose().q.x,
        //              body->getGlobalPose().q.y,
        //              body->getGlobalPose().q.z);

        // spdlog::info("position ({},{},{})", body->getGlobalPose().p.x,
        //              body->getGlobalPose().p.y - 10.0f,
        //              body->getGlobalPose().p.z);
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

grid::ObjectType grid::GameObject::get_object_type() const
{
    return object_type;
}

grid::PhysicType grid::GameObject::get_physic_type() const
{
    return physic_type;
}
