#pragma once

#include "BaseSystem.h"
#include "Entities.h"
#include "Components/BackgroundArtComponent.h"

class BackgroundArtSystem : public BaseSystem {
private:
	glm::vec2 center{0.f, 0.f};
public:
	BackgroundArtSystem();

	void Init(GameScene* scn) override;

	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;

	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override {};
	void RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn) override;

};