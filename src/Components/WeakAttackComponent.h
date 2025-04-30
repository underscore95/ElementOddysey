#pragma once
#include "Skateboard/Scene/Components.h"
#include "box2d/box2d.h"

struct WeakAttackComponent : public Skateboard::BaseComponent
{
public:
	WeakAttackComponent(const b2Vec2& direction, float damage) : direction{ direction }, damage{ damage } {};

	float damage{ 0 };
	b2Vec2 direction;
};