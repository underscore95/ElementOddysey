#pragma once

#include <Skateboard/Scene/Components.h>

struct JumpComponent : public Skateboard::BaseComponent {
public:
	static constexpr float JUMP_COOLDOWN = 5.0;
	static constexpr float JUMP_DURATION = 0.6f;

	float TimeSinceLastJump = 0;
	float IncreaseSpeed = 4.5; // scalar
	bool Jumping = false;

	void AttemptJump(float dt) {
		TimeSinceLastJump += dt;
		if (TimeSinceLastJump >= JUMP_COOLDOWN) {
			TimeSinceLastJump = 0;
		}

		Jumping = TimeSinceLastJump <= JUMP_DURATION;
	}
};