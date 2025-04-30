#pragma once
#include "GameScene.h"

class MainLayer;

class MainMenuScene : public GameScene {
public:
	explicit MainMenuScene(const std::string& name, MainLayer* layer, const SceneData& prevScene);

	MainMenuScene() = delete;


	virtual void OnHandleInput(Skateboard::TimeManager* time) override;
	virtual void OnUpdate(Skateboard::TimeManager* time) override;
	virtual void OnRender() override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	virtual void OnImGuiRender() override;

private:
	float timeOnScreen;
	static constexpr float MIN_TIME_ON_MENU = 3;
};