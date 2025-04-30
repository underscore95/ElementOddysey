#pragma once

#include "BaseSystem.h"
#include <box2d/box2d.h>

class HealthSystem : public BaseSystem {
private:
	std::shared_ptr<b2World> world;
public:
	HealthSystem(std::shared_ptr<b2World> world) : world{ world } {}

	void Init(GameScene* scn) {};
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) {};
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;

	virtual float GetUpdatePriority() const override { return 1000.0f; }
};