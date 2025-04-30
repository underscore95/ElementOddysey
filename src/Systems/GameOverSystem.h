#pragma once

#include "BaseSystem.h"
#
class PS_TestLayer;

class GameOverSystem : public BaseSystem {
private:
	GameScene* scn;

public:
	GameOverSystem(GameScene* scn) : scn{ scn } {  }

	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override {};

	virtual float GetUpdatePriority() const override { return 100.0f; }
};