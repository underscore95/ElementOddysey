#pragma once

#include "BaseSystem.h"

class ManaRegenSystem : public BaseSystem {
private:
	float timeSinceRegen = 0.0f;
public:
	void Init(GameScene* scn) override;

	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;

	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;

};