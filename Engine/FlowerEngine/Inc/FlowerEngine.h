#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"
#include "Event.h"
#include "EventManager.h"

//GameObject
#include "TypedIds.h"
#include "GameObjectHandle.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"
#include "ModelComponent.h"

//Components
#include "AIComponent.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "RenderObjectComponent.h"
#include "MeshComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"
#include "SoundEffectComponent.h"
#include "SoundBankComponent.h"
#include "UIComponent.h"
#include "UISpriteComponent.h"
#include "UIButtonComponent.h"

//Services
#include "Service.h"
#include "AIService.h"
#include "CameraService.h"
#include "RenderService.h"
#include "PhysicsService.h"
#include "UIRenderService.h"

namespace FlowerEngine
{
    App& MainApp();
}