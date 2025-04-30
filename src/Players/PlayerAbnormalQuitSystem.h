#pragma once

#include "Systems/BaseSystem.h"
#include "Scenes/SceneData.h"

class PlayerAbnormalQuitSystem : public BaseSystem {
private:

public:
	static constexpr size_t MAX_PLAYERS = 4;

	PlayerAbnormalQuitSystem() {};

	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override {};
	void OnImGuiRender() override;
};