#pragma once
#include "PxPhysicsAPI.h"
#include "../interfaces/interface_system.h"

namespace grid
{
    class Physics_System final : public ISystem
    {
    public:
        static Physics_System* instance();
        ~Physics_System() override;
        
        void start() override;
        void update(const float& delta_time) override;
        void render() override;

        void init_physics(const bool interactive);
        physx::PxRigidDynamic* create_dynamic(
            const physx::PxTransform& t,
            const physx::PxGeometry& geometry,
            const physx::PxVec3& velocity = physx::PxVec3(0)
        ) const;
        void step_physics(bool interactive) const;
        void cleanup_physics(bool interactive) const;
        glm::vec3 debug() const;

    private:
        physx::PxDefaultAllocator g_allocator;
        physx::PxDefaultErrorCallback g_error_callback;
        physx::PxFoundation* g_foundation = nullptr;
        physx::PxPhysics* g_physics = nullptr;
        physx::PxDefaultCpuDispatcher* g_dispatcher = nullptr;
        physx::PxScene* g_scene = nullptr;
        physx::PxMaterial* g_material = nullptr;
        physx::PxPvd* g_pvd = nullptr;
    };
}
