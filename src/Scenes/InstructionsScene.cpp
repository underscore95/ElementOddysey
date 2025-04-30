#include "InstructionsScene.h"

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
#include "Utils/ImGuiUtils.h"

using namespace Skateboard;

InstructionsScene::InstructionsScene(const std::string& name, MainLayer* layer, SceneData& prevData) : GameScene(name, layer)
{
	data = prevData;
	InitSystems();
}

void InstructionsScene::OnHandleInput(Skateboard::TimeManager* time)
{
	HandleInputSystems(time);

	auto users = Skateboard::Platform::GetPlatform().GetUserManager()->GetUsersByType(Skateboard::UserType::SKTB_USER_LOGIN_PLAYER);
	for (const auto& user : users) {
		const auto& id = user.id;
		if (Skateboard::Input::IsButtonPressed(Skateboard::GamePadButton_::Pad_Button_L1, id)) {
			QueueSceneChange(StateNames::MainMenu);
			return;
		}
	}
}

void InstructionsScene::OnUpdate(Skateboard::TimeManager* time)
{
	UpdateSystems(time);

	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void InstructionsScene::OnRender()
{
}

void InstructionsScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void InstructionsScene::OnImGuiRender()
{
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Instructions", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	OnImGuiRenderSystems();

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_50"));
	ImGui::SetCursorPosY(300);
	TextCentered("Element Oddysey is a game inspired by vampire survivors,", ImVec4(0, 0, 0, 1));
	TextCentered("your goal is to kill enemies and survive as long as possible!", ImVec4(0, 0, 0, 1));
	TextCentered("Save your attacks - the more you attack, the slower your attack speed!", ImVec4(0, 0, 0, 1));
	ImGui::SetCursorPosY(500);
	TextCentered("CONTROLS:", ImVec4(0, 0, 0, 1));
	TextCentered("R2 - Attack", ImVec4(0, 0, 0, 1));
	TextCentered("D pad - Special attack", ImVec4(0, 0, 0, 1));
	TextCentered("Right joystick - Movement", ImVec4(0, 0, 0, 1));
	TextCentered("@ - Shop", ImVec4(0, 0, 0, 1));
	ImGui::SetCursorPosY(800);
	TextCentered("L1 - Go back to main menu", ImVec4(0, 0, 0, 1));
	ImGui::PopFont();

	ImGui::End();
}
