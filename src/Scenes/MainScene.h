#pragma once
#include "GameScene.h"

class MainLayer;

class MainScene : public GameScene {
public:
	explicit MainScene(const std::string& name, MainLayer* layer, const SceneData& prevScene);

	MainScene() = delete;
	~MainScene();

	virtual void OnHandleInput(Skateboard::TimeManager* time) override;
	virtual void OnUpdate(Skateboard::TimeManager* time) override;
	virtual void OnRender() override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	virtual void OnImGuiRender() override;

	virtual void OnPreSceneExit() override;

private:
	float timeAlive = 0;
};