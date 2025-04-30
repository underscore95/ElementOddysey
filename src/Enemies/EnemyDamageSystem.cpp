#include "EnemyDamageSystem.h"
#include "Components/PlayerComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"

void EnemyDamageSystem::Init(GameScene* scn)
{
	
}

void EnemyDamageSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void EnemyDamageSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
}

void EnemyDamageSystem::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	auto* player = getWithComponent<PlayerComponent>(ent1, ent2);
	auto* enemy = getWithComponent<EnemyComponent>(ent1, ent2);

	if (player == nullptr || enemy == nullptr) return; // not player enemy collision

	auto& health = player->GetComponent<HealthComponent>();
	auto& enemyComp = enemy->GetComponent<EnemyComponent>();
	auto& playerComp = player->GetComponent<PlayerComponent>();

	if (!enemyComp.canAttack) return;

	const auto now = std::chrono::system_clock::now();
	if (now - health.GetLastDamagedTimestamp() >= INVINCIBILITY_TIME) {
		health.Damage(enemyComp.damage);
		Skateboard::Input::SetSimpleVibration(0.15f, 0.15f, 250, playerComp.getUserId());
		Skateboard::AudioRenderer::PlaySFX("player_hit_sfx", false);
	}
}
