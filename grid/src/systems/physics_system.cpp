#include "physics_system.h"

#define PX_FOUNDATION_VERSION_MAJOR 1
#define PX_FOUNDATION_VERSION_MINOR 0
#define PX_FOUNDATION_VERSION_BUGFIX 0

#define PX_FOUNDATION_VERSION                                                                                          \
((PX_FOUNDATION_VERSION_MAJOR << 24) + (PX_FOUNDATION_VERSION_MINOR << 16) + (PX_FOUNDATION_VERSION_BUGFIX << 8) + 0)

#define MAX_NUM_ACTOR_SHAPES 128
#define PVD_HOST "127.0.0.1"

grid::Physics_System* grid::Physics_System::instance()
{
    static Physics_System instance;
    return &instance;
}

grid::Physics_System::Physics_System()
{
    init_physics(true);
}

grid::Physics_System::~Physics_System()
{
}

void grid::Physics_System::start()
{
    ISystem::start();
}

void grid::Physics_System::update(const float& delta_time)
{
    ISystem::update(delta_time);
}

void grid::Physics_System::render()
{
    ISystem::render();
}

void grid::Physics_System::init_physics(const bool interactive)
{
    g_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_allocator, g_error_callback);

    g_pvd = PxCreatePvd(*g_foundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    g_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

    g_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_foundation, physx::PxTolerancesScale(), true, g_pvd);

    physx::PxSceneDesc scene_desc(g_physics->getTolerancesScale());
    scene_desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    g_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    scene_desc.cpuDispatcher = g_dispatcher;
    scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;
    g_scene = g_physics->createScene(scene_desc);

    physx::PxPvdSceneClient* pvd_client = g_scene->getScenePvdClient();
    if (pvd_client)
    {
        pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    g_material = g_physics->createMaterial(0.5f, 0.5f, 0.6f);

    physx::PxRigidStatic* ground_plane = PxCreatePlane(*g_physics, physx::PxPlane(0, 1, 0, 0), *g_material);
    g_scene->addActor(*ground_plane);
}

physx::PxRigidDynamic* grid::Physics_System::create_dynamic(const physx::PxTransform& t,
                                                            const physx::PxGeometry& geometry,
                                                            const physx::PxVec3& velocity)
{
    physx::PxRigidDynamic* dynamic = PxCreateDynamic(*g_physics, t, geometry, *g_material, 10.0f);
    dynamic->setAngularDamping(0.5f);
    dynamic->setLinearVelocity(velocity);
    g_scene->addActor(*dynamic);
    m_body.push_back(dynamic);
    return dynamic;
}

void grid::Physics_System::step_physics(const bool interactive) const
{
    PX_UNUSED(interactive);
    g_scene->simulate(1.0f / 60.0f);
    g_scene->fetchResults(true);
}

void grid::Physics_System::cleanup_physics(const bool interactive) const
{
    PX_UNUSED(interactive);
    g_scene->release();
    g_dispatcher->release();
    g_physics->release();
    physx::PxPvdTransport* transport = g_pvd->getTransport();
    g_pvd->release();
    transport->release();
    g_foundation->release();
}

glm::vec3 grid::Physics_System::debug()
{
    // physx::PxScene* scene;
    // PxGetPhysics().getScenes(&scene, 1);
    // const physx::PxU32 nb_actors = scene->getNbActors(
    //     physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
    // if (nb_actors)
    // {
    //     std::vector<physx::PxRigidActor*> actors(nb_actors);
    //     scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC,
    //                      reinterpret_cast<physx::PxActor**>(&actors[0]), nb_actors);
    //     //renderActors(&actors[0], static_cast<PxU32>(actors.size()));
    //
    //     // SUELO
    //
    //     for (physx::PxU32 i = 1; i < static_cast<physx::PxU32>(actors.size()); i++)
    //     {
    //         physx::PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
    //         const physx::PxU32 nb_shapes = actors[i]->getNbShapes();
    //         // spdlog::info("nbShapes: {}", actors.size());
    //         PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
    //         actors[i]->getShapes(shapes, nb_shapes);
    //         // bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;
    //
    //         for (physx::PxU32 j = 0; j < nb_shapes; j++)
    //         {
    //             const physx::PxMat44 shape_pose(physx::PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
    //             physx::PxGeometryHolder h = shapes[j]->getGeometry();
    //
    //             if (shapes[j]->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
    //                 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //
    //
    //             // spdlog::info("position ({},{},{})", shape_pose.getPosition().x,
    //             //              shape_pose.getPosition().y - 10.0f,
    //             //              shape_pose.getPosition().z);
    //
    //             return {
    //                 shape_pose.getPosition().x,
    //                 shape_pose.getPosition().y - 10.0f,
    //                 shape_pose.getPosition().z
    //             };
    //         }
    //     }
    // }

    for (physx::PxRigidDynamic*& body : m_body)
    {
        return {
            body->getGlobalPose().p.x,
            body->getGlobalPose().p.y - 10.0f,
            body->getGlobalPose().p.z
        };
    }

    return {};
}
