#include "ManaRegenSystem.h"
#include "Components/ManaComponent.h"

void ManaRegenSystem::Init(GameScene* scn)
{
}

void ManaRegenSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void ManaRegenSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	timeSinceRegen += time->DeltaTime();
	if (timeSinceRegen >= 1) {
		timeSinceRegen -= 1;
		auto view = Skateboard::ECS::GetRegistry().view<ManaComponent>();
		for (auto& ent : view) {
			auto& mana = view.get<ManaComponent>(ent);
			mana.ReplenishMana(mana.ManaRegen);
		}
	}
}
