#include "DespawnSystem.h"
#include "Components/DespawnComponent.h"
#include "Components/HealthComponent.h"

void DespawnSystem::Init(GameScene* scn)
{
}

void DespawnSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void DespawnSystem::RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto view = Skateboard::ECS::GetRegistry().view<DespawnComponent, HealthComponent>();
	for (auto& ent : view) {
		auto [despawnComp, healthComp] = view.get<DespawnComponent, HealthComponent>(ent);
		if (despawnComp.shouldDespawn()) {
			healthComp.Kill();
		}
	}
}
