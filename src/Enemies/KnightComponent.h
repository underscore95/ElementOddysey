#pragma once

#include <Skateboard/Scene/Components.h>
#include <chrono>

class KnightComponent : public Skateboard::BaseComponent {
private:

public:
	int defaultTrack;
	int attackTrack;
	float distanceToPlayer = 100;
	std::chrono::system_clock::duration attackCooldown{ std::chrono::milliseconds(1500) };
	std::chrono::system_clock::duration attackDuration{ std::chrono::milliseconds(1500) };
	std::chrono::system_clock::time_point lastStartAttacked{ std::chrono::system_clock::now() };
	std::chrono::system_clock::time_point lastEndAttacked{ std::chrono::system_clock::now() };
	bool isAttacking{ false };
};