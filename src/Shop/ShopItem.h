#pragma once

#include <Skateboard.h>
#include <Skateboard/Input.h>

typedef unsigned int ShopCategory;
static constexpr ShopCategory OFFENSIVE = 0;
static constexpr ShopCategory DEFENSIVE = 1;

static constexpr ShopCategory FIRST = OFFENSIVE;
static constexpr ShopCategory LAST = DEFENSIVE;

class ShopItem {
public:
	ShopCategory Category;
	unsigned int Price;
	std::function<void()> OnPurchase;
	std::string Name;
	std::vector<std::string> Description;

	ShopItem(const ShopCategory& category, unsigned int price,
		const std::string& name, const std::vector<std::string>& description, const std::function<void()>& onPurchase);

	void DrawGui(bool selected, bool canAfford) const;
};