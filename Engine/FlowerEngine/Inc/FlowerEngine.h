#pragma once

#include "Common.h"

#include "App.h"
#include "AppState.h"
#include "Event.h"
#include "EventManager.h"

//GameObject
#include "TypedIds.h"
#include "GameObject.h"
#include "GameWorld.h"

//Components
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"

//Services
#include "Service.h"

namespace FlowerEngine
{
    App& MainApp();
}