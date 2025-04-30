#include "Entities.h"
#include <Skateboard/Scene/Components.h>
#include "Components/BackgroundArtComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"

static constexpr const float BACKGROUND_Z = 20.0f;

Skateboard::Entity Entities::InitBackgroundEntity(const glm::vec2 offset) {
	auto ent = Skateboard::ECS::CreateEntity("background");

	auto& sprite = ent.AddComponent<Skateboard::SpriteComponent>();
	sprite.SetTexture(Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/background.png", "background"));
	sprite.SetSize(glm::vec2(BackgroundArtComponent::WIDTH, BackgroundArtComponent::HEIGHT));

	auto& trans = ent.AddComponent<Skateboard::TransformComponent>();
	trans.Trans.Translation.z = BACKGROUND_Z;

	auto& bg = ent.AddComponent<BackgroundArtComponent>();
	bg.offset = offset;

	Sprites::Add(ent);

	return ent;
}