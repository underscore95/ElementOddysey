#pragma once

#include <Skateboard.h>
#include <box2d/box2d.h>
#include <Skateboard/Scene/SceneBuilder.h>
#include <Skateboard/Assets/AssetManager.h>
#include "Elements/Element.h"
#include "Utils/Sprites.h"

/*
* Entity Initializer Namespace
* 
* Method implementations should be placed in Entities directory.
*/
namespace Entities {
	// A player
	Skateboard::Entity InitPlayer(b2World* world, UserID user, const Element& element);

	// Example camera from week 5 lab
	Skateboard::Entity InitCamera();
	// Example box from week 5 lab
	Skateboard::Entity InitBox(b2World* world);
	// Example floor from week 5 lab
	Skateboard::Entity InitFloor(b2World* world);

	// Simple test enemy
	Skateboard::Entity InitEnemy(b2World* world, const glm::vec3& position);

	Skateboard::Entity InitBatEnemy(b2World* world, const glm::vec3& position);

	Skateboard::Entity InitWolfEnemy(b2World* world, const glm::vec3& position);

	Skateboard::Entity InitKnightEnemy(b2World* world, const glm::vec3& position);

	Skateboard::Entity InitWeakAttack(b2World* world, const glm::float3& position, const glm::float2& velocity, float damage, Element element);

	Skateboard::Entity InitBackgroundEntity(const glm::vec2 offset);

	Skateboard::Entity InitStrongAttack(b2World* world, const glm::float3& position, const glm::float3& velocity, Element element);
}
