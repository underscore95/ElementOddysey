#pragma once
#include "Skateboard/Scene/Components.h"

struct StrongAttackStatModifiers
{

	float DamageScalar = 1.0f;
	float DamageAddition = 0.0f;

};
struct StrongAttackComponent : public Skateboard::BaseComponent
{

public:
	float moveSpeed = 2.0f;
	float damage = 200.0f;
	StrongAttackStatModifiers stats;
};