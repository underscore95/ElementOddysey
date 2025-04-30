#pragma once

#include "Systems/BaseSystem.h"
#include <chrono>
#include <box2d/box2d.h>
#include <memory>

class KnightSystem :
	public BaseSystem
{
private:

public:
	void Init(GameScene* scn) override {};
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override {};
	void RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override {};
	void OnImGuiRender() override {};
};

