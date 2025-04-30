#include "TemplateScene.h"

#include "Skateboard/Camera/Camera.h"
#include "Skateboard/Scene/SceneBuilder.h"
#include "Skateboard/Time/TimeManager.h"
#include "Skateboard/Assets/AssetManager.h"
#include "Skateboard/Input.h"
#include "Skateboard/Renderer/Renderer.h"
#include "Skateboard/Renderer/Renderer2D.h"
#include "MainLayer.h"

// Inlcude additional Components
#include "Skateboard/Renderer/SpriteComponent.h"

// Include Systems


// Include Entity Initializer
#include "Entities.h"

using namespace Skateboard;

TemplateScene::TemplateScene(const std::string& name, MainLayer* layer) : GameScene(name, layer)
{
	InitSystems();
}

void TemplateScene::OnHandleInput(Skateboard::TimeManager* time)
{
	HandleInputSystems(time);
}

void TemplateScene::OnUpdate(Skateboard::TimeManager* time)
{
	UpdateSystems(time);

	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void TemplateScene::OnRender()
{
	GameScene::OnRender(); // YOU MAY NOT WANT THIS!
}

void TemplateScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void TemplateScene::OnImGuiRender()
{
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Template", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	OnImGuiRenderSystems();

	ImGui::End();
}
