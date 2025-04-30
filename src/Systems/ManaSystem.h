#pragma once
#include "BaseSystem.h"
#include <box2d/box2d.h>

class ManaSystem :
    public BaseSystem
{

private:
	std::shared_ptr<b2World> world;
public:
	ManaSystem(std::shared_ptr<b2World> world) : world{ world } {}

	void Init(GameScene* scn) {};
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) {};
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
};



