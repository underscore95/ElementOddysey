#include "ShopItem.h"

ShopItem::ShopItem(const ShopCategory& category, unsigned int price,
	const std::string& name, const std::vector<std::string>& description, const std::function<void()>& onPurchase)
	: Category{ category }, Price{ price }, OnPurchase{ onPurchase }, Name{ name }, Description{ description }
{
}

void ShopItem::DrawGui(bool selected, bool canAfford) const
{
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial"));
	ImGui::TextColored(ImVec4(1, 1, 1, 1), Name.c_str());
	ImGui::PopFont();

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_50"));
	std::stringstream ss;
	ss << "Price: " << Price << " Souls";
	if (!canAfford) {
		ss << " (Too Expensive!)";
	}
	std::string priceText = ss.str();
	ImGui::TextColored(canAfford ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), priceText.c_str());

	if (canAfford && selected) {
		std::string buyText = "Press A to buy.";
		ImGui::Text(buyText.c_str());
	}
	ImGui::PopFont();

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_35"));
	for (const auto& line : Description) {
		ImGui::TextColored(ImVec4(1, 1, 1, 1), line.c_str());
	}
	ImGui::PopFont();
}
