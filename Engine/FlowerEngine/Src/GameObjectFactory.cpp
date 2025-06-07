#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "GameObject.h"
#include "GameWorld.h"

#include "Component.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"
#include "UITextComponent.h"
#include "UISpriteComponent.h"
#include "UIButtonComponent.h"

using namespace FlowerEngine;

namespace
{
    CustomComponentCB TryMake;
    CustomComponentCB TryGet;

    Component* AddComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;
        if (componentName == "CameraComponent")
        {
            newComponent = gameObject.AddComponent<CameraComponent>();
        }
        else if (componentName == "FPSCameraComponent")
        {
            newComponent = gameObject.AddComponent<FPSCameraComponent>();
        }
        else if (componentName == "TransformComponent")
        {
            newComponent = gameObject.AddComponent<TransformComponent>();
        }
        else if (componentName == "MeshComponent")
        {
            newComponent = gameObject.AddComponent<MeshComponent>();
        }
        else if (componentName == "ModelComponent")
        {
            newComponent = gameObject.AddComponent<ModelComponent>();
        }
        else if (componentName == "AnimatorComponent")
        {
            newComponent = gameObject.AddComponent<AnimatorComponent>();
        }
        else if (componentName == "RigidBodyComponent")
        {
            newComponent = gameObject.AddComponent<RigidBodyComponent>();
        }
        else if (componentName == "SoundEffectComponent")
        {
            newComponent = gameObject.AddComponent<SoundEffectComponent>();
        }
        else if (componentName == "SoundBankComponent")
        {
            newComponent = gameObject.AddComponent<SoundBankComponent>();
        }
        else if (componentName == "UITextComponent")
        {
            newComponent = gameObject.AddComponent<UITextComponent>();
        }
        else if (componentName == "UISpriteComponent")
        {
            newComponent = gameObject.AddComponent<UISpriteComponent>();
        }
        else if (componentName == "UIButtonComponent")
        {
            newComponent = gameObject.AddComponent<UIButtonComponent>();
        }
        else
        {
            newComponent = TryMake(componentName, gameObject);
            ASSERT(newComponent != nullptr, "GameObjectFactory: component [%s] is not valid", componentName.c_str());
        }

        return newComponent;
    }

    Component* GetComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* component = nullptr;
        if (componentName == "CameraComponent")
        {
            component = gameObject.GetComponent<CameraComponent>();
        }
        else if (componentName == "FPSCameraComponent")
        {
            component = gameObject.GetComponent<FPSCameraComponent>();
        }
        else if (componentName == "TransformComponent")
        {
            component = gameObject.GetComponent<TransformComponent>();
        }
        else if (componentName == "MeshComponent")
        {
            component = gameObject.GetComponent<MeshComponent>();
        }
        else if (componentName == "ModelComponent")
        {
            component = gameObject.GetComponent<ModelComponent>();
        }
        else if (componentName == "AnimatorComponent")
        {
            component = gameObject.GetComponent<AnimatorComponent>();
        }
        else if (componentName == "RigidBodyComponent")
        {
            component = gameObject.GetComponent<RigidBodyComponent>();
        }
        else if (componentName == "SoundEffectComponent")
        {
            component = gameObject.GetComponent<SoundEffectComponent>();
        }
        else if (componentName == "SoundBankComponent")
        {
            component = gameObject.GetComponent<SoundBankComponent>();
        }
        else if (componentName == "UITextComponent")
        {
            component = gameObject.GetComponent<UITextComponent>();
        }
        else if (componentName == "UISpriteComponent")
        {
            component = gameObject.GetComponent<UISpriteComponent>();
        }
        else if (componentName == "UIButtonComponent")
        {
            component = gameObject.GetComponent<UIButtonComponent>();
        }
        else
        {
            component = TryGet(componentName, gameObject);
            ASSERT(component != nullptr, "GameObjectFactory: component [%s] is not valid", componentName.c_str());
        }

        return component;
    }
}


void GameObjectFactory::SetCustomMake(CustomComponentCB callback)
{
    TryMake = callback;
}

void GameObjectFactory::SetCustomGet(CustomComponentCB callback)
{
    TryGet = callback;
}

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& gameWorld)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");
    ASSERT(err == 0, "GameObjectFactory: failed to open file %s", templatePath.u8string().c_str());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);
    auto components = doc["Components"].GetObj();
    for (auto& component : components)
    {
        // create the component
        // add the component
        // apply the data
        Component* newComponent = AddComponent(component.name.GetString(), gameObject);
        if (newComponent != nullptr)
        {
            newComponent->Deserialize(component.value);
        }
    }
    if (doc.HasMember("Children"))
    {
        auto children = doc["Children"].GetObj();
        for (auto& child : children)
        {
            std::string name = child.name.GetString();
            std::filesystem::path chileTemplate = child.value["Template"].GetString();
            GameObject* childGO = gameWorld.CreateGameObject(name, chileTemplate);

            GameObjectFactory::OverrideDeserialize(child.value, *childGO);
            gameObject.AddChild(childGO);
            childGO->SetParent(&gameObject);
        }
    }
}

void GameObjectFactory::OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject)
{
    if (value.HasMember("Components"))
    {
        auto components = value["Components"].GetObj();
        for (auto& component : components)
        {
            Component* ownedComponent = GetComponent(component.name.GetString(), gameObject);
            if (ownedComponent != nullptr)
            {
                ownedComponent->Deserialize(component.value);
            }
        }
    }
}
