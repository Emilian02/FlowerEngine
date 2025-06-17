#include "Precompiled.h"
#include "AIService.h"

#include "AIComponent.h"

using namespace FlowerEngine;

void AIService::Initialize()
{
    mAIWorld.Initialize();
}

void AIService::Terminate()
{

}

void AIService::Update(float deltaTime)
{

}

void AIService::RegisterAIEntity(AI::Entity* entity)
{
    mAIWorld.Register(entity);
}

void AIService::UnregisterAIEntity(AI::Entity* entity)
{
    mAIWorld.Unregister(entity);
}

const AI::EntityPtrs& AIService::GetAIEntities() const
{
    return mAIWorld.GetEntities();
}

uint32_t AIService::GetNextAIId()
{
    return mAIWorld.GetNextId();
}

//void AIService::SpawnAgent()
//{
//    auto agent = std::make_unique<AIComponent>();
//    agent->Initialize();
//    scvAgents.push_back(std::move(agent));
//}
//
//void AIService::KillAgent()
//{
//    if (!scvAgents.empty())
//    {
//        scvAgents.back()->Terminate();
//        scvAgents.pop_back();
//    }
//}
//
//std::vector<std::unique_ptr<AIComponent>>& AIService::GetAgents()
//{
//    return scvAgents;
//}
//
//const std::vector<std::unique_ptr<AIComponent>>& AIService::GetAgents() const
//{
//    return scvAgents;
//}

const AI::AIWorld& AIService::GetAIWorld() const
{
    return mAIWorld;
}

AI::AIWorld& AIService::GetAIWorld()
{
    return mAIWorld;
}