#include "Precompiled.h"
#include "DebugUI.h"

#include "GraphicsSystem.h"
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

using namespace FlowerEngine;
using namespace FlowerEngine::Graphics;
using namespace FlowerEngine::Core;


void DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	if (docking)
	{
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // this enables docking
	}
	if (multiViewport)
	{
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // this enables multi viewport
	}

	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	ImGui_ImplDX11_Init(device, context);
	ImGui_ImplWin32_Init(window);
}

void DebugUI::StaticTerminate()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::SetTheme(Theme theme)
{
	switch (theme)
	{
	case Theme::Classic:    ImGui::StyleColorsClassic();
		break;
	case Theme::Dark:       ImGui::StyleColorsDark();
		break;
	case Theme::Light:      ImGui::StyleColorsLight();
		break;
	default:
		break;
	}
}

void DebugUI::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// update and rendering additional data
	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
