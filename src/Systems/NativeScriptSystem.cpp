#include "NativeScriptSystem.h"
#include "Skateboard/Scene/Components.h"
#include "Skateboard/Scene/Scene.h"
#include "Skateboard/Input.h"

void NativeScriptSystem::Init(GameScene* scn)
{
}

void NativeScriptSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
	auto group = Skateboard::ECS::GetRegistry().view<Skateboard::NativeScriptComponent>();
	for (auto entity : group) {
		auto& script = group.get<Skateboard::NativeScriptComponent>(entity);
		script.OnHandleInputFunction(time);
	}
}

void NativeScriptSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto group = Skateboard::ECS::GetRegistry().view<Skateboard::NativeScriptComponent>();
	for (auto entity : group) {
		auto& script = group.get<Skateboard::NativeScriptComponent>(entity);
		script.OnUpdateFunction(time);
	}
}
