#include "GameOverSystem.h"
#include "Components/PlayerComponent.h"
#include "Components/HealthComponent.h"
#include "MainLayer.h"

void GameOverSystem::Init(GameScene* scn)
{
}

void GameOverSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void GameOverSystem::RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto view = Skateboard::ECS::GetRegistry().view<PlayerComponent, HealthComponent>();
	for (auto& player : view) {
		auto& healthComp = view.get<HealthComponent>(player);
		if (healthComp.IsAlive()) return; // Someone is alive, not game over
	}

	// No players alive
	scn->QueueSceneChange(StateNames::GameOver);
}
