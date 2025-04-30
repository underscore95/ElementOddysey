#pragma once

#include "Enemies/JumpComponent.h"
#include "Systems/BaseSystem.h"

class JumpSystem : public BaseSystem {
public:
	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override {};
};