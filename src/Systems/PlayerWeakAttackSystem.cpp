#include "PlayerWeakAttackSystem.h"
#include "Components/HealthComponent.h"
#include "Components/WeakAttackComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Enemies/EnemyComponent.h"
#include <Skateboard/Scene/Components.h>
#include "Utils/ComponentUtils.h"
#include "Scenes/SceneData.h"

const float inverseLerp(const float min, const float max, const float value) {
	return std::clamp((value - min) / (max - value), 0.0f, 1.0f); // https://www.gamedev.net/articles/programming/general-and-gameplay-programming/inverse-lerp-a-super-useful-yet-often-overlooked-function-r5230/
}

void PlayerWeakAttackSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
	auto view = Skateboard::ECS::GetRegistry().view<PlayerComponent, Skateboard::TransformComponent>();
	for (auto ent : view) {
		auto [player, trans] = view.get<PlayerComponent, Skateboard::TransformComponent>(ent);
		player.SecondsSinceLastWeakAttack += time->DeltaTime();

		// Track time button is held
		if (Skateboard::Input::IsButtonDown(Skateboard::GamePadButton_::Pad_Button_R2, player.getUserId())) {
			player.WeakAttackTimeHeld += time->DeltaTime();
		}
		else {
			player.WeakAttackTimeHeld -= time->DeltaTime() * 4.0f;
			if (player.WeakAttackTimeHeld < 0) player.WeakAttackTimeHeld = 0;
			continue;
		}

		const float penaltyAmount = inverseLerp(0, ATTACK_SPEED_MAX_PENALTY_TIME, player.WeakAttackTimeHeld);

		// Haptic feedback
		Skateboard::SKTBDeviceHapticSettings settings;
		settings.side = Skateboard::Side_::Right;
		settings.mode = Skateboard::SKTBDPadTriggerEffectMode::SKTBD_PAD_TRIGGER_EFFECT_MODE_FEEDBACK;
		settings.commandData.feedbackParam.position = 0;
		settings.commandData.feedbackParam.strength = 2 + 6 * penaltyAmount;

		Skateboard::Input::SetHapticResponse(settings, player.getUserId());

		// Can attack?
		const float playerAttackSpeed = ATTACK_SPEED + ATTACK_SPEED * ATTACK_SPEED_MAX_PENALTY * penaltyAmount;
		if (playerAttackSpeed <= player.SecondsSinceLastWeakAttack) {
			player.SecondsSinceLastWeakAttack = 0;
			Entities::InitWeakAttack(world.get(), trans.Trans.Translation, player.Direction * SPEED, player.Damage, player.element);
		}
	}
}

void PlayerWeakAttackSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto view = Skateboard::ECS::GetRegistry().view<WeakAttackComponent, RigidBody2D>();
	for (auto ent : view) {
		auto [atk, rigid] = view.get<WeakAttackComponent, RigidBody2D>(ent);
		rigid.Body->SetLinearVelocity(atk.direction);
	}
}

void PlayerWeakAttackSystem::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	auto* enemy = getWithComponent<EnemyComponent>(ent1, ent2);
	auto* attack = getWithComponent<WeakAttackComponent>(ent1, ent2);

	if (enemy == nullptr || attack == nullptr) return; // not enemy attack collision

	auto& enemyComp = enemy->GetComponent<EnemyComponent>();
	auto& attackComp = attack->GetComponent<WeakAttackComponent>();

	enemy->GetComponent<HealthComponent>().Damage(attackComp.damage);
	attack->GetComponent<HealthComponent>().Kill();

	SceneData::Souls += enemyComp.souls;
}
