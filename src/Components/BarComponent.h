#pragma once

#include <Skateboard/Scene/Components.h>
#include <Skateboard.h>

// Useful for health bars
class BarComponent : public Skateboard::BaseComponent {
private:
public:
	float min;
	float max;
	float current;
	ImVec2 dimensions;

	/*
	* \param min When current value is <= to min, the bar will be empty
	* \param max When current value is >= to min, the bar will be full
	* \param current Current value
	*/
	BarComponent(const float min, const float max, const float current) : min{ min }, max{ max }, current{ current }, dimensions(200, 50)
	{}
};