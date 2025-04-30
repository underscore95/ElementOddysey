#include "EnemyMovementSystem.h"
#include "Skateboard/Scene/ECS.h"
#include "Skateboard/Scene/Entity.h"
#include "Components/HealthComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PlayerComponent.h"
#include "EnemyComponent.h"
#include "Enemies/JumpComponent.h"
#include "Enemies/KnightComponent.h"

using namespace Skateboard;

void EnemyMovementSystem::GetPlayerPositions()
{
	PlayerPositions.clear();
	auto players = ECS::GetRegistry().view<TransformComponent, PlayerComponent>();
	for (auto entity : players) {
		auto& transform = players.get<TransformComponent>(entity);
		PlayerPositions.push_back(transform.Trans.Translation);
	}
}

const glm::vec3 EnemyMovementSystem::GetClosestPlayer(const glm::vec3& position)
{
	glm::vec3 closestPos(0, 0, 0);
	float closestDist2 = FLT_MAX;

	for (auto& pos : PlayerPositions) {
		float dist2 = glm::distance2(position, pos);
		if (dist2 < closestDist2) {
			closestPos = pos;
			closestDist2 = dist2;
		}
	}

	return closestPos;
}

void EnemyMovementSystem::Init(GameScene* scn)
{
}

void EnemyMovementSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void EnemyMovementSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	GetPlayerPositions();

	auto enemies = ECS::GetRegistry().view<TransformComponent, EnemyComponent, RigidBody2D>();
	for (auto entity : enemies) {
		auto [transform, enemy, rigidBody] = enemies.get<TransformComponent, EnemyComponent, RigidBody2D>(entity);

		rigidBody.Body->SetAngularVelocity(0.0f);

		// Get target
		const auto& targetPosition = GetClosestPlayer(transform.Trans.Translation);

		// Get direction to move
		auto direction = targetPosition - transform.Trans.Translation;
		auto distanceFromTarget = glm::fastLength(direction);
		direction /= distanceFromTarget; // Normalise the direction vector

		auto theEntity = ECS::GetEntity(transform);
		if (theEntity.HasComponent<KnightComponent>()) {
			theEntity.GetComponent<KnightComponent>().distanceToPlayer = distanceFromTarget;
		}

		if (!enemy.canMove) {
			rigidBody.Body->SetLinearVelocity(b2Vec2_zero);
			continue;
		}

		// Angle to look
		if (enemy.rotates) {
			const auto angleInRadians = atan2f(direction.y, direction.x) + glm::half_pi<float>();
			rigidBody.Body->SetTransform(rigidBody.Body->GetPosition(), angleInRadians);
		}

		// Set velocity
		float speed = enemy.moveSpeed * time->DeltaTime() * 60;
		JumpComponent* jump = ECS::GetRegistry().try_get<JumpComponent>(entity);
		if (jump && jump->Jumping) {
			speed *= jump->IncreaseSpeed;
		}
		rigidBody.Body->SetLinearVelocity(b2Vec2(direction.x * speed, direction.y * speed));
	}
}
