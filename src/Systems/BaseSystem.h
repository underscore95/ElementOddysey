#pragma once

#include "Skateboard/Scene/ECS.h"
#include "Skateboard/Time/TimeManager.h"

class GameScene;
class Entity;
class BaseSystem {
public:
	virtual ~BaseSystem() = default;

	virtual void Init(GameScene* scn) = 0;
	virtual void HandleInput(Skateboard::TimeManager* time, GameScene* scn) = 0;
	virtual void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) = 0;
	virtual void RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn) {};
	virtual void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) {}
	virtual void OnImGuiRender() {};

	// Decides the order in which systems should update. Lower values update first. Default value is 0.
	virtual float GetUpdatePriority() const { return 0.0f; }
};
