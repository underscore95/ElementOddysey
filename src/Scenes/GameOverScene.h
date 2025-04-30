#pragma once
#include "GameScene.h"
#include "SceneData.h"

class MainLayer;

class GameOverScene : public GameScene {
public:
	GameOverScene(const std::string& name, MainLayer* layer, const SceneData& prevScene);
	GameOverScene() = delete;

	virtual void OnHandleInput(Skateboard::TimeManager* time) override;
	virtual void OnUpdate(Skateboard::TimeManager* time) override;
	virtual void OnRender() override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	virtual void OnImGuiRender() override;

private:
	long unsigned int score = 0;
};