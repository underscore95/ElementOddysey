#include "PlayerControllerScript.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PlayerComponent.h"

void PlayerControllerScript::OnUpdate(Skateboard::TimeManager* time)
{
}

void PlayerControllerScript::OnCreate()
{
}

void PlayerControllerScript::OnDestroy()
{
}

void PlayerControllerScript::OnHandleInput(Skateboard::TimeManager* time)
{
	constexpr const float SPEED = 200.0f;

	auto& rigidBody = GetComponent<RigidBody2D>();
	auto& player = GetComponent<PlayerComponent>();

	player.Direction = Skateboard::Input::GetRightStick(player.getUserId());

	if (abs(player.Direction.x) <= FLT_EPSILON && abs(player.Direction.y) <= FLT_EPSILON) {
		player.Direction.y = -1.0f;
		rigidBody.Body->SetLinearVelocity(b2Vec2(0, 0));
		return;
	}

	player.Direction = glm::normalize(player.Direction);

	rigidBody.Body->SetLinearVelocity(b2Vec2(player.Direction.x * SPEED * time->DeltaTime(), player.Direction.y * SPEED * time->DeltaTime()));
}
