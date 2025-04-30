#pragma once

#include "Elements/Element.h"
#include <Skateboard.h>

struct SceneData {
public:
	long unsigned int Score{ 0 };
	static unsigned int Souls;
	static unsigned int HighScore;
	std::map<UserID, Element> Players;
};