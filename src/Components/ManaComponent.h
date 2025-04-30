#pragma once

#include "Skateboard/Scene/Components.h"

class ManaComponent : public Skateboard::BaseComponent
{

private:
	float mana = 100;
	bool removeOnDeath = true;

public:

	ManaComponent(float m) : mana(m) {};

	float maxMana = 100;

	float ManaRegen = 2.0f; // mana per second
	
	// Remove mana from player
	void RemoveMana(float amount)
	{
		mana -= amount;
		if (mana < 0)
		{
			mana = 0;
		}
	}

	// Restore a certain amount of mana to player
	void ReplenishMana(float amount)
	{
		mana += amount;

		if (mana > maxMana)
		{
			mana = maxMana;
		}
	}

	// Getter for mana
	const float GetMana()
	{
		return mana;
	}
};

