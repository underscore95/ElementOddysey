#include "Sprites.h"

// TODO: A priority queue would be faster here, but the std implementation doesn't provide support for iteration, and I don't feel like writing my own
std::vector<Sprites::ZEntity> sprites;

void Sprites::Add(Skateboard::Entity entity)
{
	assert(entity.HasComponent<Skateboard::SpriteComponent>());
	assert(entity.HasComponent<Skateboard::TransformComponent>());

	ZEntity zent;

	auto& trans = entity.GetComponent<Skateboard::TransformComponent>();
	zent.z = trans.Trans.Translation.z;
	zent.ent = entity;

	for (auto it = sprites.begin(); it < sprites.end(); ++it) {
		if ((*it).z <= zent.z) {
			sprites.insert(it, zent);
			return;
		}
	}

	sprites.push_back(zent);
}

void Sprites::Remove(Skateboard::Entity entity)
{
	for (auto it = sprites.begin(); it < sprites.end(); ++it) {
		if ((*it).ent == entity) {
			sprites.erase(it);
			return;
		}
	}
}

const std::vector<Sprites::ZEntity>& Sprites::GetSprites()
{
	return sprites;
}

void Sprites::Clear()
{
	sprites.clear();
}

const bool Sprites::AreAllSpritesAdded()
{
	return sprites.size() == Skateboard::ECS::GetRegistry().view<Skateboard::SpriteComponent>().size();
}

const bool Sprites::AreAllSpritesValid()
{
	for (const auto& spr : sprites) {
		if (!Skateboard::ECS::GetRegistry().valid(spr.ent)) return false;
	}
	return true;
}
