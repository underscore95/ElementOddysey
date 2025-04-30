#pragma once

#include "Skateboard/Scene/Components.h"
#include "box2d/box2d.h"

struct RigidBody2D : public Skateboard::BaseComponent
{
public:
	b2Body* Body;
	RigidBody2D(b2Body* Body) : Body{ Body } {};
};
