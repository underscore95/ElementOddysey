#include "BackgroundArtSystem.h"

BackgroundArtSystem::BackgroundArtSystem()
{
	// Generate a grid of background tiles (5x5), once player moves too far from center, we will move them all exactly 1 background tile
	// towards the player (this isn't visible to the player)
	// but it creates the illusion of an infinite background
	for (int x = -2; x <= 2; ++x) {
		for (int y = -2; y <= 2; ++y) {
			Entities::InitBackgroundEntity(glm::vec2(BackgroundArtComponent::WIDTH * x, BackgroundArtComponent::HEIGHT * y));
		}
	}
}

void BackgroundArtSystem::Init(GameScene* scn)
{
}

void BackgroundArtSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void BackgroundArtSystem::RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto cameras = Skateboard::ECS::GetRegistry().view<Skateboard::TransformComponent, Skateboard::CameraComponent>();
	for (auto& cam : cameras) {
		auto [camComp, camTrans] = cameras.get<Skateboard::CameraComponent, Skateboard::TransformComponent>(cam);
		if (!camComp.Primary) continue;

		auto& camPos = camTrans.Trans.Translation;

		// Update center position
		while (center.x - camPos.x > BackgroundArtComponent::WIDTH) center.x -= BackgroundArtComponent::WIDTH;
		while (camPos.x - center.x > BackgroundArtComponent::WIDTH) center.x += BackgroundArtComponent::WIDTH;
		while (center.y - camPos.y > BackgroundArtComponent::HEIGHT) center.y -= BackgroundArtComponent::HEIGHT;
		while (camPos.y - center.y > BackgroundArtComponent::HEIGHT) center.y += BackgroundArtComponent::HEIGHT;

		// Update background components positions
		auto backgrounds = Skateboard::ECS::GetRegistry().view<Skateboard::TransformComponent, BackgroundArtComponent>();
		for (auto& ent : backgrounds) {
			auto [trans, background] = backgrounds.get<Skateboard::TransformComponent, BackgroundArtComponent>(ent);
			trans.Trans.Translation = glm::vec3(center.x + background.offset.x, center.y + background.offset.y, trans.Trans.Translation.z);
		}
	}
}
