#pragma once
#include "Systems/BaseSystem.h"
#include <vector>
#include <Skateboard.h>

class EnemyMovementSystem :
	public BaseSystem
{
private:
	std::vector<glm::vec3> PlayerPositions;
	void GetPlayerPositions();
	const glm::vec3 GetClosestPlayer(const glm::vec3& position);

public:
	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
};

