#pragma once

#include <Skateboard.h>
#include <Skateboard/Renderer/SpriteComponent.h>

namespace Sprites {
	struct ZEntity {
		Skateboard::Entity ent;
		float z;
	};

	void Add(Skateboard::Entity entity);
	void Remove(Skateboard::Entity entity);
	const std::vector<ZEntity>& GetSprites();
	void Clear();

	// Testing
	const bool AreAllSpritesAdded();
	const bool AreAllSpritesValid();
}