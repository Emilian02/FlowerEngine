#include "Precompiled.h"
#include "AIComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "AIService.h"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"

using namespace FlowerEngine;

void AIComponent::Initialize()
{
    AIService* aiService = GetOwner().GetWorld().GetService<AIService>();
    ASSERT(aiService != nullptr, "AIComponent: AIService not found");

    if (!mAgent)
    {
        mAgent = std::make_unique<AI::Agent>(aiService->GetAIWorld(), 0);
    }

    mSteeringModule = std::make_unique<AI::SteeringModule>(*mAgent);
    mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
    mSeekBehavior->SetActive(true);

    mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
    mFleeBehavior->SetActive(true);

    mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
    mArriveBehavior->SetActive(true);

    aiService->RegisterAIEntity(mAgent.get());
}

void AIComponent::Terminate()
{
    AIService* aiService = GetOwner().GetWorld().GetService<AIService>();
    ASSERT(aiService != nullptr, "AIComponent: AIService not found");

    if (aiService && mAgent)
    {
        aiService->UnregisterAIEntity(mAgent.get());
    }

    mSteeringModule.reset();
    mSeekBehavior = nullptr;
    mFleeBehavior = nullptr;
    mArriveBehavior = nullptr;
    mAgent.reset();
}

void AIComponent::Update(float deltatTime)
{
    Math::Vector3 force = mSteeringModule->Calculate();
    mAgent->velocity += force * deltatTime / mAgent->mass;
    mAgent->velocity = Math::ClampMagnitude(mAgent->velocity, 0 ,mAgent->maxSpeed);
    mAgent->position += mAgent->velocity * deltatTime;

    TransformComponent* transform = GetOwner().GetComponent<TransformComponent>();
    if (transform)
    {
        transform->position = mAgent->position;
    }
}

void AIComponent::DebugUI()
{
    AIService* aiService = GetOwner().GetWorld().GetService<AIService>();
    std::vector<std::unique_ptr<AIComponent>>& scvAgents = aiService->GetAgents();

    ImGui::Begin("Steering", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        if (ImGui::Button("SpawnAgent"))
        {
            aiService->SpawnAgent();
        }
        if (ImGui::Button("KillAgent"))
        {
            aiService->KillAgent();
        }

        static bool showDebug = false;
        if (ImGui::Checkbox("ShowDebug", &showDebug))
        {
            for (auto& agent : scvAgents)
            {
                agent->ShowDebug(showDebug);
            }
        }

        static bool useSeek = true;
        static float weightSeek = 1.0f;
        if (ImGui::Checkbox("Seek", &useSeek))
        {
            for (auto& agent : scvAgents) 
            {
                agent->SetSeek(useSeek);
            }
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("SeekWeight", &weightSeek, 0.1f, 0.1f, 5.0f))
        {
            for (auto& agent : scvAgents) 
            {
                agent->SetSeekWeight(weightSeek);
            }
        }

        static bool useFlee = false;
        static float weightFlee = 1.0f;
        if (ImGui::Checkbox("Flee", &useFlee))
        {
            for (auto& agent : scvAgents)
            {
                agent->SetFlee(useFlee);
            }
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("FleeWeight", &weightFlee, 0.1f, 0.1f, 5.0f))
        {
            for (auto& agent : scvAgents) 
            {
                agent->SetFleeWeight(weightFlee);
            }
        }

        static bool useArrive = false;
        static float weightArrive = 1.0f;
        if (ImGui::Checkbox("Arrive", &useArrive))
        {
            for (auto& agent : scvAgents) 
            {
                agent->SetArrive(useArrive);
            }
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("ArriveWeight", &weightArrive, 0.1f, 0.1f, 5.0f))
        {
            for (auto& agent : scvAgents)
            {
                agent->SetArriveWeight(weightArrive);
            }
        }

    }
    ImGui::End();
}

void AIComponent::Deserialize(const rapidjson::Value& value)
{
    TransformComponent* transform = GetOwner().GetComponent<TransformComponent>();
    ASSERT(transform != nullptr, "AIComponent: TransformComponent not found on GameObject");
    mAgent->position = transform->position;

    RigidBodyComponent* rigidBody = GetOwner().GetComponent<RigidBodyComponent>();
    ASSERT(rigidBody != nullptr, "AIComponent: RigidBodyComponent not found on GameObject");
    mAgent->mass = rigidBody->GetMass();

    if (value.HasMember("Behavior"))
    {
        auto arr = value["Behavior"].GetArray();
        for (const auto& b : arr)
        {
            std::string behavior = b.GetString();
            if (behavior == "Seek")
            {
                SetSeek(true);
            }
            else if (behavior == "Flee")
            {
                SetFlee(true);
            }
            else if (behavior == "Arrive")
            {
                SetArrive(true);
            }
        }
    }

    if (value.HasMember("SeekWeight"))
    {
        SetSeekWeight(value["SeekWeight"].GetFloat());
    }
    if (value.HasMember("FleeWeight"))
    {
        SetSeekWeight(value["FleeWeight"].GetFloat());
    }
    if (value.HasMember("ArriveWeight"))
    {
        SetSeekWeight(value["ArriveWeight"].GetFloat());
    }
}

void AIComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{

}

void AIComponent::ShowDebug(bool show)
{
    mSeekBehavior->ShowDebug(show);
    mFleeBehavior->ShowDebug(show);
    mArriveBehavior->ShowDebug(show);
}

void AIComponent::SetSeek(bool active)
{
    mSeekBehavior->SetActive(active);
}

void AIComponent::SetSeekWeight(float weight)
{
    mSeekBehavior->SetWeight(weight);
}

void AIComponent::SetFlee(bool active)
{
    mFleeBehavior->SetActive(active);
}

void AIComponent::SetFleeWeight(float weight)
{
    mFleeBehavior->SetWeight(weight);
}

void AIComponent::SetArrive(bool active)
{
    mArriveBehavior->SetActive(active);
}

void AIComponent::SetArriveWeight(float weight)
{
    mArriveBehavior->SetWeight(weight);
}

void AIComponent::SetDestination(const Math::Vector3& destination)
{
    mAgent->destination = destination;
}

AI::Agent* AIComponent::GetAgent()
{
    return nullptr;
}

const AI::Agent* AIComponent::GetAgent() const
{
    return nullptr;
}
