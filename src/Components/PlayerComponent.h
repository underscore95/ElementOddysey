#pragma once

#include "Skateboard/Scene/Components.h"
#include <Skateboard/User.h>
#include <chrono>
#include "Elements/Element.h"

class PlayerComponent : public Skateboard::BaseComponent {
private:
	
	UserID user;
	std::chrono::system_clock::time_point creation{ std::chrono::system_clock::now() }; // Camera will follow the oldest player

public:
	PlayerComponent(const Element& element, UserID user) : user{ user }, element{ element } {};

	const UserID getUserId() const { return user; }
	const std::chrono::system_clock::time_point& getCreationTime() const { return creation; }
	float SecondsSinceLastWeakAttack{ 100.0f };
	float WeakAttackTimeHeld{ 0.0f };
	glm::vec2 Direction{ glm::vec2(0.0f, -1.0f) };
	float Damage = 50.0f;
	Element element;
};                                                                                                                                                                                              