#include "PlayerAbnormalQuitSystem.h"
#include "Components/PlayerComponent.h"
#include "Components/HealthComponent.h"

void PlayerAbnormalQuitSystem::Init(GameScene* scn)
{
}

void PlayerAbnormalQuitSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void PlayerAbnormalQuitSystem::RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto users = Skateboard::Platform::GetPlatform().GetUserManager()->GetUsersByType(Skateboard::UserType::SKTB_USER_LOGIN_PLAYER);
	auto players = Skateboard::ECS::GetRegistry().view<HealthComponent, PlayerComponent>();
	for (auto& player : players) {
		auto [playerComp, health] = players.get<PlayerComponent, HealthComponent>(player);
		bool found = false;
		for (auto& user : users) {
			if (playerComp.getUserId() == user.id) {
				found = true;
				break;
			}
		}

		if (!found) {
			health.Kill();
		}
	}
}

void PlayerAbnormalQuitSystem::OnImGuiRender()
{
}
