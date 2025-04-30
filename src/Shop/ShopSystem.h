#pragma once

#include "Systems/BaseSystem.h"
#include "ShopItem.h"

struct Button {
	std::string Name;
	Skateboard::GamePadButton_ Btn;
	Button(const std::string& name, const Skateboard::GamePadButton_ btn) : Name{ name }, Btn{ btn } {};
};

class ShopSystem : public BaseSystem {
private:
	static constexpr ImVec2 SHOP_ITEM_DIMENSIONS = ImVec2(250, 400);

	std::vector<ShopItem> items;
	std::vector<ShopCategory> categories;

	std::unordered_map<ShopCategory, Button> buyButtons;

	void PickAvailableShopItem(ShopCategory category);
	void RenderShopItem(const size_t i);
	void TryBuyItem(const size_t i);
public:
	ShopSystem();

	void Init(GameScene* scn) {};
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void OnImGuiRender() override;
};