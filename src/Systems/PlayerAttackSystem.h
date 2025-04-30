
#pragma once

#include "BaseSystem.h"
#include <chrono>
#include <box2d/box2d.h>
#include <random>
#include <Skateboard.h>
#include <Skateboard/Scene/Components.h>
#include "Components/ManaComponent.h"
//#include <memory>

class PlayerAttackSystem : public BaseSystem
{

private:
	std::shared_ptr<b2World> world;

	template <typename T>
	Skateboard::Entity* getWithComponent(Skateboard::Entity* ent1, Skateboard::Entity* ent2) {
		if (ent1->HasComponent<T>()) return ent1;
		else if (ent2->HasComponent<T>()) return ent2;
		else return nullptr;
	}


public:
	void StrongAttack(Skateboard::TransformComponent& transform, ManaComponent& mana, float velx, float vely, float offsetx, float offsety);

	void Init(GameScene* scn) override;

	PlayerAttackSystem(std::shared_ptr<b2World> world_);

	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) override;
	void OnImGuiRender() override;



};
