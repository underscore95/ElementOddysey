#include "HealthSystem.h"
#include "Components/HealthComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Utils/Sprites.h"
#include <Skateboard/Renderer/SpriteComponent.h>

void HealthSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	using namespace Skateboard;
	auto group = ECS::GetRegistry().view<HealthComponent>();
	for (auto& ent : group) {
		const auto& health = group.get<HealthComponent>(ent);
		if (health.IsDead() && health.removeOnDeath) {

			// Remove this entity
			auto rigid = ECS::GetRegistry().try_get<RigidBody2D>(ent);
			if (rigid) {
				rigid->Body->GetWorld()->DestroyBody(rigid->Body);
			}

			if (ECS::GetRegistry().try_get<Skateboard::SpriteComponent>(ent)) {
				Sprites::Remove(ent);
			}
			ECS::GetRegistry().destroy(ent);
		}
	}
}