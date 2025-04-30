#include "MainMenuScene.h"

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
#include "Players/PlayerJoinSystem.h"

// Include Entity Initializer
#include "Entities.h"

using namespace Skateboard;

MainMenuScene::MainMenuScene(const std::string& name, MainLayer* layer, const SceneData& prevScene) : GameScene(name, layer), timeOnScreen{ 0 }
{
	data = prevScene;
	RegisterSystem(std::make_unique<PlayerJoinSystem>(data));

	InitSystems();
}

void MainMenuScene::OnHandleInput(Skateboard::TimeManager* time)
{
	HandleInputSystems(time);

	auto users = Skateboard::Platform::GetPlatform().GetUserManager()->GetUsersByType(Skateboard::UserType::SKTB_USER_LOGIN_PLAYER);
	for (const auto& user : users) {
		const auto& id = user.id;
		if (Skateboard::Input::IsButtonPressed(Skateboard::GamePadButton_::Pad_Button_L1, id)) {
			QueueSceneChange(StateNames::Instructions);
			return;
		}

		if (timeOnScreen >= MIN_TIME_ON_MENU) {
			if (Input::IsButtonDown(GamePadButton_::Pad_Button_Bottom, id)) {
				QueueSceneChange(StateNames::Game);
				return;
			}
		}
	}
}

void MainMenuScene::OnUpdate(Skateboard::TimeManager* time)
{
	timeOnScreen += time->DeltaTime();

	UpdateSystems(time);

	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void MainMenuScene::OnRender()
{

}

void MainMenuScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void MainMenuScene::OnImGuiRender()
{
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	OnImGuiRenderSystems();

	std::string highScoreText = "High Score: " + std::to_string(SceneData::HighScore);

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial"));
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 300);
	TextCentered(highScoreText.c_str(), ImVec4(0, 0, 0, 1));

	if (timeOnScreen >= MIN_TIME_ON_MENU) {
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 200);
		TextCentered("Press X to start.", ImVec4(1, 0, 0, 1));
	}

	// HOW TO PLAY
	ImGui::SetCursorPosY(200);
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_50"));
	TextCentered("Press L1 for instructions on how to play.", ImVec4(0, 0, 0, 1));
	ImGui::PopFont();

	ImGui::PopFont();

	ImGui::End();
}
