#pragma once
#include "GameScene.h"

class MainLayer;

class InstructionsScene : public GameScene {
public:
	explicit InstructionsScene(const std::string& name, MainLayer* layer, SceneData& prevData);

	InstructionsScene() = delete;


	virtual void OnHandleInput(Skateboard::TimeManager* time) override;
	virtual void OnUpdate(Skateboard::TimeManager* time) override;
	virtual void OnRender() override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	virtual void OnImGuiRender() override;

private:
};