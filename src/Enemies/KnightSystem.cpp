#include "KnightSystem.h"
#include "Enemies/EnemyComponent.h"
#include "Enemies/KnightComponent.h"
#include "Animations/SpriteAnimationComponent.h"
#include <Skateboard.h>

constexpr float SFX_DISTANCE = 4.0f;

void KnightSystem::RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	const auto now = std::chrono::system_clock::now();

	auto knights = Skateboard::ECS::GetRegistry().view<EnemyComponent, KnightComponent, SpriteAnimComponent>();
	for (auto& ent : knights) {
		auto [knight, enemy, animator] = knights.get<KnightComponent, EnemyComponent, SpriteAnimComponent>(ent);

		if (knight.isAttacking && now - knight.lastStartAttacked >= knight.attackDuration) {
			// Move to player
			enemy.canAttack = false;
			knight.isAttacking = false;
			knight.lastEndAttacked = now;
			animator.Play(knight.defaultTrack);
			enemy.canMove = true;
		}

		if (!knight.isAttacking && now - knight.lastEndAttacked >= knight.attackCooldown) {
			// Attack
			enemy.canAttack = true;
			knight.isAttacking = true;
			knight.lastStartAttacked = now;
			animator.Play(knight.attackTrack);
			if (knight.distanceToPlayer < SFX_DISTANCE) Skateboard::AudioRenderer::PlaySFX("knight_attack_sfx", false);
			enemy.canMove = false;
		}
	}
}
