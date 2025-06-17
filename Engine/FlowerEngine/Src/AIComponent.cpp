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

    mAgent = new AI::Agent(aiService->GetAIWorld(), 0);

    mSteeringModule = std::make_unique<AI::SteeringModule>(*mAgent);
    mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();

    mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();

    mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();

    mTransform = GetOwner().GetComponent<TransformComponent>();
    mAgent->position = mTransform->position;
    mRigidBody = GetOwner().GetComponent<RigidBodyComponent>();
    mAgent->mass = mRigidBody->GetMass();

    //aiService->RegisterAIEntity(mAgent);

    SetSeek(mActiveSeek);
    SetFlee(mActiveFlee);
    SetArrive(mActiveArrive);

    SetSeekWeight(mWeightSeek);
    SetFleeWeight(mWeightFlee);
    SetArriveWeight(mWeightArrive);

    mTarget.x = 5.0f;
    SetDestination(mTarget);
}

void AIComponent::Terminate()
{
    AIService* aiService = GetOwner().GetWorld().GetService<AIService>();
    ASSERT(aiService != nullptr, "AIComponent: AIService not found");

    if (aiService && mAgent)
    {
        aiService->UnregisterAIEntity(mAgent);
    }

    mSteeringModule.reset();
    mSeekBehavior = nullptr;
    mFleeBehavior = nullptr;
    mArriveBehavior = nullptr;
    mAgent = nullptr;
}

void AIComponent::Update(float deltatTime)
{
    Math::Vector3 force = mSteeringModule->Calculate();
    mAgent->velocity += force * deltatTime / mAgent->mass;
    mAgent->velocity = Math::ClampMagnitude(mAgent->velocity, 0 ,mAgent->maxSpeed);
    mAgent->position += mAgent->velocity * deltatTime;

    mRigidBody->SetVelocity(mAgent->velocity);
}

void AIComponent::DebugUI()
{
    AIService* aiService = GetOwner().GetWorld().GetService<AIService>();

    ImGui::Begin("Steering", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    {
        /*if (ImGui::Button("SpawnAgent"))
        {
            aiService->SpawnAgent();
        }
        if (ImGui::Button("KillAgent"))
        {
            aiService->KillAgent();
        }*/

        static bool showDebug = false;
        if (ImGui::Checkbox("ShowDebug", &showDebug))
        {
            ShowDebug(showDebug);
        }
        if (ImGui::DragFloat3("Target", &mTarget.x))
        {
            SetDestination(mTarget);
        }

        static float weightSeek = 1.0f;
        if (ImGui::Checkbox("Seek", &mActiveSeek))
        {
            SetSeek(mActiveSeek);
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("SeekWeight", &weightSeek, 0.1f, 0.1f, 5.0f))
        {
            SetSeekWeight(weightSeek);
        }

        static float weightFlee = 1.0f;
        if (ImGui::Checkbox("Flee", &mActiveFlee))
        {
            SetFlee(mActiveFlee);
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("FleeWeight", &weightFlee, 0.1f, 0.1f, 5.0f))
        {
            SetFleeWeight(weightFlee);
        }

        static float weightArrive = 1.0f;
        if (ImGui::Checkbox("Arrive", &mActiveArrive))
        {
            SetArrive(mActiveArrive);
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("ArriveWeight", &weightArrive, 0.1f, 0.1f, 5.0f))
        {
            SetArriveWeight(weightArrive);
        }

    }
    ImGui::End();
}

void AIComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Behavior"))
    {
        auto arr = value["Behavior"].GetArray();
        for (const auto& b : arr)
        {
            std::string behavior = b.GetString();
            if (behavior == "Seek")
            {
                //mActiveSeek = true;
            }
            else if (behavior == "Flee")
            {
                //mActiveFlee = true;
            }
            else if (behavior == "Arrive")
            {
                //mActiveArrive = true;
            }
        }
    }

    if (value.HasMember("SeekWeight"))
    {
        mWeightSeek = value["SeekWeight"].GetFloat();
    }
    if (value.HasMember("FleeWeight"))
    {
        mWeightFlee = value["FleeWeight"].GetFloat();
    }
    if (value.HasMember("ArriveWeight"))
    {
        mWeightArrive = value["ArriveWeight"].GetFloat();
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
