#include "BarSystem.h"
#include <imgui/imgui.h>
#include "Components/PlayerComponent.h"
#include "Components/BarComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"

void BarSystem::Init(GameScene* scn)
{
}

void BarSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void BarSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
}

void BarSystem::OnImGuiRender()
{
	using namespace Skateboard;
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_35"));

	auto healthBarGroup = ECS::GetRegistry().view<PlayerComponent, BarComponent, HealthComponent>();
	int i = 0;
	for (auto& ent : healthBarGroup) {
		++i;
		auto [bar, health, player] = healthBarGroup.get<BarComponent, HealthComponent, PlayerComponent>(ent);
		bar.current = health.GetHealth();
		float progress = bar.max == 0 ? 0 : bar.current / bar.max;
		if (progress < 0) progress = 0;
		else if (progress > 1) progress = 1;
		std::string playerId = "Player " + std::to_string(i) + " health";
		ImGui::TextColored(ImVec4(1, 1, 1, 1), playerId.c_str());
		ImGui::ProgressBar(progress, bar.dimensions);
	}

	auto manaBarGroup = ECS::GetRegistry().view<PlayerComponent, BarComponent, ManaComponent>();
	int j = 0;
	for (auto& ent : manaBarGroup)
	{
		++j;
		auto [bar, mana, player] = manaBarGroup.get<BarComponent, ManaComponent, PlayerComponent>(ent);
		bar.current = mana.GetMana();
		float progress = bar.max == 0 ? 0 : bar.current / bar.max;

		if (progress < 0)
		{
			progress = 0;
		}
		else if (progress > 1)
		{
			progress = 1;
		}

		std::string playerId = "Player " + std::to_string(j) + " mana";
		ImGui::TextColored(ImVec4(1, 1, 1, 1), playerId.c_str());
		ImGui::ProgressBar(progress, bar.dimensions);
	}

	ImGui::PopFont();
}
