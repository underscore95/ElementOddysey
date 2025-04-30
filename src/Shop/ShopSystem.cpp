#include "ShopSystem.h"
#include "Components/HealthComponent.h"
#include "Components/ManaComponent.h"
#include "Components/PlayerComponent.h"
#include "Utils/ImGuiUtils.h"
#include "Scenes/SceneData.h"
#include "Enemies/EnemyComponent.h"

static std::unordered_map<ShopCategory, size_t> availableOptions; // indices of available options for each category
static bool initialisedAvailableOptions = false;

void ShopSystem::PickAvailableShopItem(ShopCategory category)
{
	std::vector<size_t> options;
	for (size_t i = 0; i < items.size(); ++i) {
		ShopItem& item = items[i];
		if (item.Category != category) continue;
		options.push_back(i);
	}

	assert(!options.empty());
	availableOptions[category] = options[rand() % options.size()];
}

void ShopSystem::RenderShopItem(const size_t i)
{
	ShopItem& item = items[i];
	auto it = buyButtons.find(item.Category);
	if (it == buyButtons.end()) return;
	const std::string btnName = it->second.Name;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 5.0f);
	ImGui::SetNextWindowBgAlpha(0.5f);
	ImGui::SetNextWindowSize(SHOP_ITEM_DIMENSIONS, ImGuiCond_Once);
	const std::string windowId = "ShopItem-" + std::to_string(i);
	ImGui::Begin(windowId.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_35"));

	ImGui::TextColored(ImVec4(0, 0, 0, 1), item.Name.c_str());
	for (const auto& line : item.Description) {
		ImGui::TextColored(ImVec4(0, 0, 0, 1), line.c_str());
	}

	ImGui::SetCursorPosY(SHOP_ITEM_DIMENSIONS.y - 80);

	std::string costText = "Cost: " + std::to_string(item.Price) + " Souls";
	ImGui::TextColored(ImVec4(item.Price > SceneData::Souls, 0, 0, 1), costText.c_str());

	std::string buyText = "Press " + btnName + " to purchase";
	ImGui::TextColored(ImVec4(0, 0, 0, 1), buyText.c_str());

	ImGui::PopFont();

	ImGui::End();
	ImGui::PopStyleVar();
}

void ShopSystem::TryBuyItem(const size_t i)
{
	auto& item = items[i];
	if (SceneData::Souls >= item.Price) {
		// Can afford
		SceneData::Souls -= item.Price;
		item.OnPurchase();
		PickAvailableShopItem(item.Category);
	}
}

ShopSystem::ShopSystem()
{
	for (auto i = FIRST; i <= LAST; ++i) {
		categories.push_back(i);
	}

	{
		std::vector<std::string> desc = { "Heal all players to", "maximum health." };
		items.emplace_back(DEFENSIVE, 10, "Full Heal", desc, [&] {
			auto view = Skateboard::ECS::GetRegistry().view<HealthComponent, PlayerComponent>();
			for (auto& player : view) {
				auto& health = view.get<HealthComponent>(player);
				health.Heal();
			}
			});
	}

	{
		std::vector<std::string> desc = { "Heal all players by", "10 HP" };
		items.emplace_back(DEFENSIVE, 1, "Minor Heal", desc, [&] {
			auto view = Skateboard::ECS::GetRegistry().view<HealthComponent, PlayerComponent>();
			for (auto& player : view) {
				auto& health = view.get<HealthComponent>(player);
				health.Heal(10);
			}
			});
	}

	{
		std::vector<std::string> desc = { "Replenish players", "mana to maximum." };
		items.emplace_back(DEFENSIVE, 10, "Full Restore", desc, [&] {
			auto view = Skateboard::ECS::GetRegistry().view<ManaComponent, PlayerComponent>();
			for (auto& player : view) {
				auto& mana = view.get<ManaComponent>(player);
				mana.ReplenishMana(100);
			}
			});
	}


	for (int damageAmount = 10; damageAmount <= 30; damageAmount += 10) {
		std::stringstream ss;
		ss << "players by " << damageAmount << ".";
		std::vector<std::string> desc = { "Increase the ATK DMG", "of all", ss.str() };
		items.emplace_back(OFFENSIVE, 5, "Upgrade Attack", desc, [damageAmount] {
			auto view = Skateboard::ECS::GetRegistry().view<PlayerComponent>();
			for (auto& player : view) {
				auto& playerComp = view.get<PlayerComponent>(player);
				playerComp.Damage += damageAmount;
			}
			});
	}

	{
		std::vector<std::string> desc = { "Deal 10 Damage", "to all enemies." };
		items.emplace_back(OFFENSIVE, 10, "Weak Nuke", desc, [&] {
			auto view = Skateboard::ECS::GetRegistry().view<HealthComponent, EnemyComponent>();
			for (auto& enemy : view) {
				auto& health = view.get<HealthComponent>(enemy);
				health.Damage(10);
			}
			});
	}

	if (!initialisedAvailableOptions) {
		for (auto category : categories) {
			PickAvailableShopItem(category);
		}
		initialisedAvailableOptions = true;
	}

	buyButtons.emplace(DEFENSIVE, Button("X", Skateboard::GamePadButton_::Pad_Button_Bottom));
	buyButtons.emplace(OFFENSIVE, Button("O", Skateboard::GamePadButton_::Pad_Button_Right));
	assert(buyButtons.size() == categories.size());
}

void ShopSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
	for (auto& pair : availableOptions) {
		auto category = pair.first;
		auto it = buyButtons.find(category);
		if (it == buyButtons.end()) continue;
		auto view = Skateboard::ECS::GetRegistry().view<PlayerComponent>();
		auto i = pair.second;
		for (auto& ent : view) {
			auto& player = view.get<PlayerComponent>(ent);
			if (!Skateboard::Input::IsButtonPressed(it->second.Btn, player.getUserId())) continue;
			TryBuyItem(i);
		}
	}
}

void ShopSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
}

void ShopSystem::OnImGuiRender()
{
	float offset = 500;
	for (auto& pair : availableOptions) {
		auto i = pair.second;

		ImGui::SetNextWindowPos(ImVec2{ offset, 400.0f }, ImGuiCond_Once);
		RenderShopItem(i);

		offset += 500;
	}
}
