#include "SplashScreenScene.h"

#include "Skateboard/Camera/Camera.h"
#include "Skateboard/Scene/SceneBuilder.h"
#include "Skateboard/Time/TimeManager.h"
#include "Skateboard/Assets/AssetManager.h"
#include "Skateboard/Input.h"
#include "Skateboard/Renderer/Renderer.h"
#include "Skateboard/Renderer/Renderer2D.h"
#include "MainLayer.h"
#include "Utils/ImGuiUtils.h"

// Inlcude additional Components
#include "Skateboard/Renderer/SpriteComponent.h"

// Include Systems


// Include Entity Initializer
#include "Entities.h"

using namespace Skateboard;

SplashScreenScene::SplashScreenScene(const std::string& name, MainLayer* layer) : GameScene(name, layer)
{
	InitSystems();
}

void SplashScreenScene::OnHandleInput(Skateboard::TimeManager* time)
{
	HandleInputSystems(time);
}

void SplashScreenScene::OnUpdate(Skateboard::TimeManager* time)
{
	UpdateSystems(time);

	timeOnScene += time->DeltaTime();

	if (timeOnScene >= SPLASH_DURATION) {
		QueueSceneChange(StateNames::MainMenu);
		timeOnScene = SPLASH_DURATION;
	}

	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void SplashScreenScene::OnRender()
{

}

void SplashScreenScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void SplashScreenScene::OnImGuiRender()
{
	static constexpr const float HEIGHT = 1080;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5.0f);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, HEIGHT }, ImGuiCond_Once);
	ImGui::Begin("SplashScreenUI", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	OnImGuiRenderSystems();

	ImGui::PushFont(Skateboard::AssetManager::GetFont("splash"));
	ImGui::SetCursorPosY(HEIGHT * (1.0 - 0.5 * timeOnScene / TEXT_MOVE_SPEED));
	TextCentered("Element Odyssey", ImVec4(1, 1, 1, 1));
	ImGui::PopFont();

	ImGui::End();
	ImGui::PopStyleVar();
}
