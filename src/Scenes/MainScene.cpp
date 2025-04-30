#include "MainScene.h"

#include "Skateboard/Camera/Camera.h"
#include "Skateboard/Scene/SceneBuilder.h"
#include "Skateboard/Time/TimeManager.h"
#include "Skateboard/Assets/AssetManager.h"
#include "Skateboard/Input.h"
#include "Skateboard/Renderer/Renderer.h"
#include "Skateboard/Renderer/Renderer2D.h"
#include "MainLayer.h"
#include "Utils/ImGuiUtils.h"
#include "Utils/Sprites.h"

// Inlcude additional Components
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Scripts/PlayerControllerScript.h"
#include "Components/BarComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/HealthComponent.h"

// Include Systems
#include "Systems/CameraSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Enemies/EnemyMovementSystem.h"
#include "Systems/NativeScriptSystem.h"
#include "Enemies/WaveSystem.h"
#include "Systems/BarSystem.h"
#include "Systems/HealthSystem.h"
#include "Enemies/EnemyDamageSystem.h"
#include "Animations/SpriteAnimationSystem.h"
#include "Systems/DespawnSystem.h"
#include "Enemies/JumpSystem.h"
#include "Systems/GameOverSystem.h"
#include "Enemies/KnightSystem.h"
#include "Systems/BackgroundArtSystem.h"
#include "Players/PlayerAbnormalQuitSystem.h"
#include "Systems/PlayerAttackSystem.h"
#include "Systems/PlayerWeakAttackSystem.h"
#include "Systems/ManaRegenSystem.h"

// Include Entity Initializer
#include "Entities.h"

using namespace Skateboard;

MainScene::MainScene(const std::string& name, MainLayer* layer, const SceneData& prevScene) : GameScene(name, layer)
{
	music = GAME_MUSIC;
	SceneData::Souls = 0;

	// Define gravity
	b2Vec2 gravity(0, 0);

	// Create world
	auto world = std::make_shared<b2World>(gravity);

	// Use Entity Initializer for initializing entities.
	Entities::InitCamera();

	// Creating systems
	RegisterSystem(std::make_unique<PhysicsSystem2D>(world));

	RegisterSystem(std::make_unique<CameraSystem>());
	RegisterSystem(std::make_unique<EnemyMovementSystem>());
	RegisterSystem(std::make_unique<NativeScriptSystem>());
	RegisterSystem(std::make_unique<WaveSystem>(world));
	RegisterSystem(std::make_unique<BarSystem>());
	RegisterSystem(std::make_unique<EnemyDamageSystem>());
	RegisterSystem(std::make_unique<SpriteAnimationSystem>());
	RegisterSystem(std::make_unique<DespawnSystem>());
	RegisterSystem(std::make_unique<JumpSystem>());
	RegisterSystem(std::make_unique<GameOverSystem>(this));
	RegisterSystem(std::make_unique<KnightSystem>());
	RegisterSystem(std::make_unique<BackgroundArtSystem>());
	RegisterSystem(std::make_unique<PlayerAbnormalQuitSystem>());
	RegisterSystem(std::make_unique<ManaRegenSystem>());
	RegisterSystem(std::make_unique<PlayerAttackSystem>(world));
	RegisterSystem(std::make_unique<PlayerWeakAttackSystem>(world));

	RegisterSystem(std::make_unique<HealthSystem>(world));

	InitSystems();

	for (auto& player : prevScene.Players) {
		Entities::InitPlayer(world.get(), player.first, player.second); // Make a new player
	}
}

MainScene::~MainScene()
{
	Sprites::Clear();

	auto entities = ECS::GetRegistry().view<TagComponent>();
	for (auto& entity : entities) {
		ECS::GetRegistry().destroy(entity);
	}
}

void MainScene::OnHandleInput(TimeManager* time)
{
	Scene::OnHandleInput(time);
	
	HandleInputSystems(time);

	auto view = ECS::GetRegistry().view<PlayerComponent>();
	for (auto& player : view) {
		auto& comp = view.get<PlayerComponent>(player);
		if (Skateboard::Input::IsButtonPressed(Skateboard::GamePadButton_::Pad_Button_Top)) {
			QueueSceneChange(StateNames::Shop);
		}
	}
}

void MainScene::OnUpdate(TimeManager* time)
{
	Scene::OnUpdate(time);
	
	UpdateSystems(time);

	// Update Camera to renderer
	{

		auto cameraGroup = ECS::GetRegistry().view<CameraComponent>();
		for (auto entity : cameraGroup)
		{
			// Get the renderer and mesh component from each entity
			auto& cameraComponent = cameraGroup.get<CameraComponent>(entity);
			if (cameraComponent.Primary)
				UpdateCameraBuffers(*cameraComponent.Camera);

		}
	}

	static constexpr double SCORE_PER_SECOND = 10;
	timeAlive += time->DeltaTime();
	data.Score = timeAlive * SCORE_PER_SECOND;

	if (SceneChangeQueued) {
		SceneChangeQueued = false;
		Layer->ChangeScene(SceneChange);
		return;
	}
}

void MainScene::OnRender()
{
	GameScene::OnRender();
}

void MainScene::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	OnCollisionSystems(ent1, ent2);
}

void MainScene::OnImGuiRender()
{
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Game UI", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	OnImGuiRenderSystems();

	// SCORE
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_35"));
	{
		std::stringstream ss;
		ss << "Score: " << data.Score;
		std::string scoreText = ss.str();
		ImGui::TextColored(ImVec4(1, 1, 1, 1), scoreText.c_str());
	}


	// SOULS
	{
		std::stringstream ss;
		ss << "Souls: " << data.Souls;
		std::string soulsText = ss.str();
		ImGui::TextColored(ImVec4(1, 1, 1, 1), soulsText.c_str());
	}
	ImGui::PopFont();

	// OPEN SHOP
	ImGui::SetCursorPosY(100);
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_50"));
	TextCentered("Press @ to open shop.", ImVec4(1, 1, 1, 1));
	ImGui::PopFont();

	ImGui::End();
}

void MainScene::OnPreSceneExit()
{
}
