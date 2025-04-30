#pragma once

#include "BaseSystem.h"

class BarSystem : public BaseSystem {
private:
public:
	void Init(GameScene* scn) override;

	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;

	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;

	void OnImGuiRender() override;

};