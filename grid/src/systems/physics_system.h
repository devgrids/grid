#pragma once
#include "PxPhysicsAPI.h"
#include "../interfaces/interface_system.h"
#include <vector>

namespace grid
{
    class Physics_System final : public ISystem
    {
    public:
        static Physics_System* instance();
        Physics_System();
        ~Physics_System() override;

        void start() override;
        void update(const float& delta_time) override;
        void render() override;
        
        physx::PxRigidDynamic* create_dynamic(
            const physx::PxTransform& t,
            const physx::PxGeometry& geometry,
            const physx::PxVec3& velocity = physx::PxVec3(0)
        );
        void cleanup_physics(bool interactive = true) const;
        glm::vec3 debug();

    private:
        physx::PxDefaultAllocator g_allocator;
        physx::PxDefaultErrorCallback g_error_callback;
        physx::PxFoundation* g_foundation = nullptr;
        physx::PxPhysics* g_physics = nullptr;
        physx::PxDefaultCpuDispatcher* g_dispatcher = nullptr;
        physx::PxScene* g_scene = nullptr;
        physx::PxMaterial* g_material = nullptr;
        physx::PxPvd* g_pvd = nullptr;

        std::vector<physx::PxRigidDynamic*> m_body;
    };
}
