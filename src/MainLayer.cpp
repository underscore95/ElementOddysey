#include "MainLayer.h"
#include "Skateboard/Assets/AssetManager.h"
#include "Skateboard/Scene/SceneBuilder.h"
#include "Scenes/MainScene.h"
#include "MainLayer.h"
#include "Scenes/GameOverScene.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/ShopScene.h"
#include "Scenes/SplashScreenScene.h"
#include "Scenes/InstructionsScene.h"
#include "Utils/MusicManager.h"

MainLayer::MainLayer()
{
	Skateboard::AssetManager::LoadFont(L"/app0/assets/fonts/Arial.otf", 75, "arial");
	Skateboard::AssetManager::LoadFont(L"/app0/assets/fonts/Arial.otf", 50, "arial_50");
	Skateboard::AssetManager::LoadFont(L"/app0/assets/fonts/Arial.otf", 25, "arial_35");
	Skateboard::AssetManager::LoadFont(L"/app0/assets/fonts/creepster.ttf", 100, "lose");
	Skateboard::AssetManager::LoadFont(L"/app0/assets/fonts/creepster.ttf", 200, "splash");
	Skateboard::AssetManager::LoadAudio(L"/app0/assets/audio/player/player_hit.wav", "player_hit_sfx");
	Skateboard::AssetManager::LoadAudio(L"/app0/assets/audio/enemies/wolf_spawn.wav", "wolf_spawn_sfx");
	Skateboard::AssetManager::LoadAudio(L"/app0/assets/audio/enemies/knight_attack_sfx.wav", "knight_attack_sfx");

	p_CurrentScene = std::make_unique<SplashScreenScene>("Splash", this);
	p_CurrentScene->stateName = StateNames::SplashScreen;
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnEvent(Skateboard::Event& e)
{
}

void MainLayer::OnResize(int newClientWidth, int newClientHeight)
{
}

bool MainLayer::OnHandleInput(Skateboard::TimeManager* time)
{
	p_CurrentScene->OnHandleInput(time);

	return true;
}

bool MainLayer::OnUpdate(Skateboard::TimeManager* time)
{
	// Prepare for rendering
	p_CurrentScene->OnUpdate(time);

	MusicManager::Singleton().Update(time);

	return true;
}

void MainLayer::OnRender()
{
	Skateboard::Renderer::StartDraw();

	// Render the scene and all entities currently active.
	p_CurrentScene->OnRender();

	Skateboard::Renderer::EndDraw();

}

void MainLayer::OnImGuiRender()
{

	// Create the main Render Window
	ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 1920, 1080 }, ImGuiCond_Once);
	ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoTitleBar);
	ImGui::Image(p_CurrentScene->GetSceneAlbedo()->GetRenderTargetAsImGuiTextureID(), { 1920, 1080 });
	ImGui::SameLine();
	//ImGui::Image(m_FrameBuffer->GetDepthStencilTargetAsImGuiTextureID(), { 720, 480 });
	//ImGui::Image((ImTextureID)m_StoneTexture->GetTexture(), { 720, 480 });
	ImGui::End();


	// Create the console output window
	//ImGui::SetNextWindowPos(ImVec2{ 32, 32 }, ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2{ 400, 200 }, ImGuiCond_Once);
	//ImGui::Begin("Test Layer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	//ImGui::Text("PS5 Controller - %f", 0.0);
	////ImGui::Text("Cam P - %f  %f  %f", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
	////ImGui::Text("Cam R - %f  %f  %f", m_Camera.GetRotation().x, m_Camera.GetRotation().y, m_Camera.GetRotation().z);			

	//ImGui::End();

	p_CurrentScene->OnImGuiRender();

	// Render Scene Hiearachy (Uses Imui)
	//m_SceneHierarchy.OnImGuiRender();
}

void MainLayer::ChangeScene(StateNames newScene)
{
	StateNames currentName = p_CurrentScene->stateName;

	// Temporary fix  to switch between scenes
	if (currentName == newScene)
		newScene = StateNames::Game;
	if (currentName != newScene)
	{
		p_CurrentScene->OnPreSceneExit();
		SceneData prevScene = p_CurrentScene->data; // Need to copy this

		if (ShouldResetData(currentName, newScene)) {
			if (mainScene == p_CurrentScene) {
				mainScene.reset();
			}
			// Clear current scene. If moving to or from some scenes, clearning can be avoided (for example, pause scene)
			p_CurrentScene->OnSceneExit();

			p_CurrentScene.reset();
		}

		// Switch statement to decide which scene to go into. 
		switch (newScene)
		{
		case StateNames::GameOver:
			p_CurrentScene = std::make_unique<GameOverScene>("Game Over", this, prevScene);
			break;
		case StateNames::Game:
			if (mainScene) {
				p_CurrentScene = mainScene;
			}
			else {
				p_CurrentScene = std::make_unique<MainScene>("Game", this, prevScene);
				mainScene = p_CurrentScene;
			}
			break;
		case StateNames::MainMenu:
			p_CurrentScene = std::make_unique<MainMenuScene>("Main Menu", this, prevScene);
			break;
		case StateNames::Shop:
			p_CurrentScene = std::make_unique<ShopScene>("Shop", this, prevScene);
			break;
		case StateNames::Instructions:
			p_CurrentScene = std::make_unique<InstructionsScene>("Instructions", this, prevScene);
			break;
		default:
			assert(false); // You forgot to add a case for your scene state
			break;
		}

		p_CurrentScene->stateName = newScene;

		MusicManager::Singleton().SetMusic(p_CurrentScene->music);
	}
}