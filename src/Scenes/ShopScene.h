#pragma once
#include "GameScene.h"

class MainLayer;

class ShopScene : public GameScene {
public:
	explicit ShopScene(const std::string& name, MainLayer* layer, SceneData prevScene);

	ShopScene() = delete;

	virtual void OnHandleInput(Skateboard::TimeManager* time) override;
	virtual void OnUpdate(Skateboard::TimeManager* time) override;
	virtual void OnRender() override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	virtual void OnImGuiRender() override;

private:
};