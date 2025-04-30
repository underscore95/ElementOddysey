#include "GameOverScene.h"

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

GameOverScene::GameOverScene(const std::string& name, MainLayer* layer, const SceneData& prevScene) : GameScene(name, layer),
score{ prevScene.Score }
{
	music = LOSE_MUSIC;
	if (score > SceneData::HighScore) {
		SceneData::HighScore = score;
	}
	InitSystems();
}

void GameOverScene::OnHandleInput(Skateboard::TimeManager* time)
{
	HandleInputSystems(time);

	auto users = Skateboard::Platform::GetPlatform().GetUserManager()->GetUsersByType(Skateboard::UserType::SKTB_USER_LOGIN_PLAYER);
	for (const auto& user : users) {
		const auto& id = user.id;
		if (Input::IsButtonDown(GamePadButton_::Pad_Button_Bottom, id)) {
			Layer->ChangeScene(StateNames::MainMenu);
			break;
		}
	}
}

void GameOverScene::OnUpdate(Skateboard::TimeManager* time)
{
	UpdateSystems(time);

	// TODO Orthographic camera
	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void GameOverScene::OnRender()
{

}

void GameOverScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void GameOverScene::OnImGuiRender()
{
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	OnImGuiRenderSystems();

	ImGui::PushFont(AssetManager::GetFont("lose"));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
	TextCentered("Game Over!", ImVec4(1, 0, 0, 1));
	ImGui::PopFont();

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial"));
	std::string scoreText = "Score: " + std::to_string(score);
	TextCentered(scoreText.c_str(), ImVec4(1, 0, 0, 1));
	std::string highScoreText = "High Score: " + std::to_string(SceneData::HighScore);
	TextCentered(highScoreText.c_str(), ImVec4(1, 0, 0, 1));
	TextCentered("Press X to continue.", ImVec4(1, 1, 1, 1));
	ImGui::PopFont();

	ImGui::End();
}
