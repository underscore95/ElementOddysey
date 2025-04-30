#pragma once

#include "Systems/BaseSystem.h"
#include "sktbdpch.h"

class SpriteAnimationSystem :
	public BaseSystem
{
	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
};
