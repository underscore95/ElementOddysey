#include "CameraSystem.h"
#include "Skateboard/Scene/Components.h"
#include "Skateboard/Scene/Scene.h"
#include "Skateboard/Input.h"
#include "Components/PlayerComponent.h"

void CameraSystem::UpdateCamPosition(Transform& trans, float deltaTime)
{
	auto players = Skateboard::ECS::GetRegistry().view<PlayerComponent, Skateboard::TransformComponent>();

	// Find oldest player
	auto& playerTrans = trans; // Default value will result in no camera movement if there is no players
	std::chrono::system_clock::time_point oldest = std::chrono::system_clock::now();
	for (auto& ent : players) {
		auto [player, thisPlayerTrans] = players.get<PlayerComponent, Skateboard::TransformComponent>(ent);
		if (player.getCreationTime() <= oldest) {
			playerTrans = thisPlayerTrans.Trans;
			oldest = player.getCreationTime();
		}
	}

	trans.Translation = playerTrans.Translation + glm::vec3(0, 0, -10);
}

void CameraSystem::Init(GameScene* scn)
{
}

void CameraSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}


void CameraSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto& registry = Skateboard::ECS::GetRegistry();


	auto group = registry.view<Skateboard::TransformComponent, Skateboard::CameraComponent>();
	for (auto entity : group) {
		auto [transform, camera] = group.get<Skateboard::TransformComponent, Skateboard::CameraComponent>(entity);

		if (scn && camera.Primary)
		{
			// If the Scene has been provided, then update camera buffers for that scene with the primary camera


			Skateboard::OrthographicCamera* perspCam = dynamic_cast<Skateboard::OrthographicCamera*>(camera.Camera.get());

			UpdateCamPosition(transform, time->DeltaTime());

			//else if (OrthogrpahicCamera)
				// Update Orthographic camera ...

			// Update the camera view matrix  based on the transform component of the entity
			camera.Camera->UpdateViewMatrix(transform);
		}
	}

}
