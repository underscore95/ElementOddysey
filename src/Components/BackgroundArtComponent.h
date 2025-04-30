#pragma once

#include <glm/glm.hpp>
#include <Skateboard/Scene/Components.h>

class BackgroundArtComponent : public Skateboard::BaseComponent {
public:
	// Size of the bg sprites
	static constexpr const float WIDTH = 20;
	static constexpr const float HEIGHT = 20;
	glm::vec2 offset{ glm::zero<glm::vec2>() };
};