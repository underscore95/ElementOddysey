#pragma once
#include "Systems/BaseSystem.h"
#include <vector>
#include <Skateboard.h>
#include <Skateboard/Scene/Components.h>
#include "Utils/ComponentUtils.h"

class EnemyDamageSystem :
	public BaseSystem
{
private:
	static constexpr auto INVINCIBILITY_TIME = std::chrono::milliseconds(750);
public:
	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override; 

	virtual float GetUpdatePriority() const override { return 100.0f; }
};