#include "JumpSystem.h"

void JumpSystem::Init(GameScene* scn)
{
}

void JumpSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void JumpSystem::RunParallelUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	const auto dt = time->DeltaTime();
	auto& reg = Skateboard::ECS::GetRegistry();

	auto view = reg.view<JumpComponent>();
	for (auto& ent : view) {
		auto& jump = view.get<JumpComponent>(ent);
		jump.AttemptJump(dt);
	}
}
