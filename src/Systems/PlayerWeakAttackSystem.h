#pragma once

#include "BaseSystem.h"
#include "Entities.h"
#include "Components/PlayerComponent.h"

class PlayerWeakAttackSystem : public BaseSystem {
private:
	static const constexpr float SPEED = 10.0f;
	static const constexpr float ATTACK_SPEED = 1.0f / 5.0f; // 0.2s cd
	static const constexpr float ATTACK_SPEED_MAX_PENALTY = 0.85f; // attack speed += attack speed * penalty * penaltyPercent
	static const constexpr float ATTACK_SPEED_MAX_PENALTY_TIME = 5.0f;
	std::shared_ptr<b2World> world;
public:
	PlayerWeakAttackSystem(std::shared_ptr<b2World> world) : world{ world } {};

	void Init(GameScene* scn) override {};

	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;

	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;

	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;

};