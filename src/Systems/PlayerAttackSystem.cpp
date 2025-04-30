#include "PlayerAttackSystem.h"
#include <Skateboard/Log.h>
#include "Entities.h"
#include "Components/PlayerComponent.h"
#include <Skateboard/Scene/Components.h>
#include <algorithm>
#include "Skateboard/Scene/ECS.h"
#include "Skateboard/Scene/Entity.h"
#include "Components/PhysicsBodyComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"
#include "Components/StrongAttackComponent.h"


using namespace Skateboard;


void PlayerAttackSystem::StrongAttack(Skateboard::TransformComponent& transform, ManaComponent& mana, float velx, float vely, float offsetx, float offsety)
{
	auto players = Skateboard::ECS::GetRegistry().view<PlayerComponent, ManaComponent, Skateboard::TransformComponent>();

	for (auto& ent : players) {
		auto& transform = players.get<Skateboard::TransformComponent>(ent);
		auto& mana = players.get<ManaComponent>(ent);
		auto& player = players.get<PlayerComponent>(ent);

		float3 position = transform.Trans.Translation;

		// Set attack velocity based on direction
		float3 vel(velx, vely, 0);

		// Add offset to player position
		position.x += offsetx;
		position.y += offsety;

		// Check if player has enough mana to strong attack
		if (mana.GetMana() >= 50)
		{
			// Initialise strong attack
			Entities::InitStrongAttack(world.get(), position, vel, player.element);

			// Remove mana from player
			mana.RemoveMana(50);
		}
	}
	// Check if player has enough mana to strong attack

}

void PlayerAttackSystem::Init(GameScene* scn)
{
}

PlayerAttackSystem::PlayerAttackSystem(std::shared_ptr<b2World> world_)
{
	world = world_;
}

void PlayerAttackSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void PlayerAttackSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{

	auto players = Skateboard::ECS::GetRegistry().view<PlayerComponent, ManaComponent, Skateboard::TransformComponent>();

	for (auto& ent : players)
	{
		auto [player, mana, transform] = players.get<PlayerComponent, ManaComponent, Skateboard::TransformComponent>(ent);

		// Right
		if (Input::IsButtonReleased(0x00000020, player.getUserId()))
		{
			StrongAttack(transform, mana, 5, 0, 1, 0);
		}

		// Left
		if (Input::IsButtonReleased(0x00000080, player.getUserId()))
		{


			StrongAttack(transform, mana, -5, 0, -1, 0);
		}

		// Down
		if (Input::IsButtonReleased(0x00000040, player.getUserId()))
		{

			StrongAttack(transform, mana, 0, -5, 0, -1);
		}

		// Up
		if (Input::IsButtonReleased(0x00000010, player.getUserId()))
		{

			StrongAttack(transform, mana, 0, 5, 0, 1);
		}
	}
}

void PlayerAttackSystem::OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	// Get enemy and attack components
	auto* enemy = getWithComponent<EnemyComponent>(ent1, ent2);
	auto* strongAttack = getWithComponent<StrongAttackComponent>(ent1, ent2);

	// Check if enemy collided with strong attack
	if (enemy == nullptr || strongAttack == nullptr) return;

	// Get enemy and strong attack health
	auto& health = enemy->GetComponent<HealthComponent>();
	auto& strongAttackHealth = strongAttack->GetComponent<HealthComponent>();

	auto& strongAttackComp = strongAttack->GetComponent<StrongAttackComponent>();

	// Damage enemy
	health.Damage(strongAttackComp.damage);

	// Damage strong attack (can pierce 4 enemies before despawning)
	strongAttackHealth.Damage(25);
}

void PlayerAttackSystem::OnImGuiRender()
{
}


