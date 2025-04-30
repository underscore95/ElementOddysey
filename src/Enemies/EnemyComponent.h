#pragma once
#include "Skateboard/Scene/Components.h"

struct StatModifiers {
	// All enemies are given base stats for each wave, and then this is applied on top
	float HealthScalar = 1.0f;
	float HealthAddition = 0.0f;
	float DamageScalar = 1.0f;
	float DamageAddition = 0.0f;
};

struct EnemyComponent : public Skateboard::BaseComponent
{
public:
	float moveSpeed = 1.0f;
	float damage = 10.0f;
	StatModifiers stats;
	bool canAttack = true; // If true, enemy deals damage
	bool canMove = true; // If true, enemy can move
	bool rotates = true; // If true, rotates towards nearest enemy
	int souls = 1;
};

