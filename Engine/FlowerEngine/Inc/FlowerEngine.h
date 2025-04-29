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
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "RenderObjectComponent.h"
#include "MeshComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"

//Services
#include "Service.h"
#include "CameraService.h"
#include "RenderService.h"
#include "PhysicsService.h"

namespace FlowerEngine
{
    App& MainApp();
}