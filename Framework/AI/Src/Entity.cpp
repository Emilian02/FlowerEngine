#include "Precompiled.h"  
#include "Entity.h"  

#include "AIWorld.h"  

using namespace FlowerEngine;
using namespace FlowerEngine::AI;

Entity::Entity(AIWorld& world, uint32_t typeId)  
    : world(world)  
    , mUniqueId(static_cast<uint64_t>(typeId) << 32 | world.GetNextId())  
{  
    world.Register(this);  
}  

Entity::~Entity()  
{  
    world.Unregister(this);  
}  

Math::Matrix4 AI::Entity::GetWorldTransform() const  
{
    const Math::Vector3 l = heading;
    const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
    const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
    const Math::Vector3 d = position;

    return
    {
        r.x, r.y, r.z, 0.0f,
        u.x, u.y, u.z, 0.0f,
        l.x, l.y, l.z, 0.0f,
        d.x, d.y, d.z, 1.0f
    };
}
