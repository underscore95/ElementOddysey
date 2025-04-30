#pragma once

#include <Skateboard/Scene/Components.h>
#include <Skateboard.h>
#include <chrono>

class DespawnComponent : public Skateboard::BaseComponent {
public:
	DespawnComponent(std::chrono::system_clock::time_point despawnTime) : despawnTime{ despawnTime } {}
	DespawnComponent(std::chrono::system_clock::duration duration) : despawnTime{ duration + std::chrono::system_clock::now() } {}

	std::chrono::system_clock::time_point despawnTime;

	bool shouldDespawn() {
		return despawnTime < std::chrono::system_clock::now();
	}
};