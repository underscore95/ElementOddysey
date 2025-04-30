#pragma once
#include "Skateboard/Scene/Components.h"
#include <chrono>

class HealthComponent : public Skateboard::BaseComponent {
private:
	float health = 100;
	std::chrono::system_clock::time_point lastDamaged = std::chrono::system_clock::now();
public:
	HealthComponent(float h) : health(h) {};

	float maxHealth = 100;
	bool removeOnDeath = true;

	/*
	* Set player health to 0 and updates lastDamaged
	*/
	void Kill() { 
		health = 0;
		lastDamaged = std::chrono::system_clock::now();
	}

	/*
	* Remove amount from the players health and updates lastDamaged.
	* 
	* \param amount Amount to damage
	*/
	void Damage(const float amount) { 
		assert(amount >= 0);
		health -= amount; 
		lastDamaged = std::chrono::system_clock::now();
	}

	/*
	* Increases the health by amount. If amount is a negative value, sets health to max health.
	* 
	* \param amount Amount to heal
	*/
	void Heal(const float amount = -1) {
		if (amount < 0) health = maxHealth;
		else health += amount;
	}
	
	const float GetHealth() const { return health; }
	const std::chrono::system_clock::time_point& GetLastDamagedTimestamp() const { return lastDamaged; }
	const bool IsAlive() const { return health > 0; }
	const bool IsDead() const { 	return health <= 0; }
};