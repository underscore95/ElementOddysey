#pragma once

enum class StateNames {
	MainMenu = 0,
	Shop = 1,
	GameOver = 2,
	Game = 3,
	SplashScreen = 4,
	Instructions = 5
};

inline static bool ShouldResetData(StateNames prev, StateNames to) {
	if (prev == StateNames::Game && to == StateNames::Shop) {
		return false;
	}

	return true;
}
