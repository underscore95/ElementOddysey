#include "ShopScene.h"

#include "Skateboard/Camera/Camera.h"
#include "Skateboard/Scene/SceneBuilder.h"
#include "Skateboard/Time/TimeManager.h"
#include "Skateboard/Assets/AssetManager.h"
#include "Skateboard/Input.h"
#include "Skateboard/Renderer/Renderer.h"
#include "Skateboard/Renderer/Renderer2D.h"
#include "MainLayer.h"
#include "Components/PlayerComponent.h"

#include "Utils/ImGuiUtils.h"

// Inlcude additional Components
#include "Skateboard/Renderer/SpriteComponent.h"

// Include Systems
#include "Shop/ShopSystem.h"

// Include Entity Initializer
#include "Entities.h"

using namespace Skateboard;

/*
NOTE!
Due to how "persistent" scenes are implemented, the entities in the game scene still exist, so don't render/update them here.
If you need to, adding a component storing what scene an entity is from would be a fix
*/

ShopScene::ShopScene(const std::string& name, MainLayer* layer, SceneData prevScene) : GameScene(name, layer)
{
	RegisterSystem(std::make_unique<ShopSystem>());

	InitSystems();
}

void ShopScene::OnHandleInput(Skateboard::TimeManager* time)
{
	HandleInputSystems(time);

	auto view = ECS::GetRegistry().view<PlayerComponent>();
	for (auto& player : view) {
		auto& comp = view.get<PlayerComponent>(player);
		if (Skateboard::Input::IsButtonPressed(Skateboard::GamePadButton_::Pad_Button_Top, comp.getUserId())) {
			QueueSceneChange(StateNames::Game);
		}
	}
}

void ShopScene::OnUpdate(Skateboard::TimeManager* time)
{
	UpdateSystems(time);

	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void ShopScene::OnRender()
{

}

void ShopScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void ShopScene::OnImGuiRender()
{
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Shop", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_35"));
	// SOULS
	std::stringstream ss;
	ss << "Souls: " << data.Souls;
	std::string soulsText = ss.str();
	ImGui::TextColored(ImVec4(0, 0, 0, 1), soulsText.c_str());
	ImGui::PopFont();

	OnImGuiRenderSystems();

	ImGui::SetCursorPosY(100);
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_50"));
	TextCentered("Press @ to close shop.", ImVec4(0, 0, 0, 1));
	ImGui::PopFont();

	ImGui::End();
}
