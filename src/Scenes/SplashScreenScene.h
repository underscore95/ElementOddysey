#pragma once
#include "GameScene.h"

class MainLayer;

class SplashScreenScene : public GameScene {
private:
	static constexpr const float SPLASH_DURATION = 5.0f; // seconds
	static constexpr const float TEXT_MOVE_SPEED = 3.0f; // it moves half the screen height every this time in seconds

	float timeOnScene = 0.0f;
public:
	explicit SplashScreenScene(const std::string& name, MainLayer* layer);

	SplashScreenScene() = delete;

	virtual void OnHandleInput(Skateboard::TimeManager* time) override;
	virtual void OnUpdate(Skateboard::TimeManager* time) override;
	virtual void OnRender() override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	virtual void OnImGuiRender() override;
};