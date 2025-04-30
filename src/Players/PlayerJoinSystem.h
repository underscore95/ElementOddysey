#pragma once

#include "Systems/BaseSystem.h"
#include "Scenes/SceneData.h"

class PlayerJoinSystem : public BaseSystem {
private:
	SceneData& Data;
public:
	static constexpr size_t MAX_PLAYERS = 4;

	PlayerJoinSystem(SceneData& data) : Data{ data } {};

	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void OnImGuiRender() override;
};