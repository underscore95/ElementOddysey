#pragma once

#include "Systems/BaseSystem.h"
#include <chrono>
#include <box2d/box2d.h>
#include <random>
#include <memory>

class WaveSystem :
	public BaseSystem
{
private:
	static constexpr float WAVE_DURATION = 30;
	static constexpr unsigned int MAX_ENEMIES = 50;

	unsigned int wave{ 0 }; // Current wave
	std::shared_ptr<b2World> world;
	unsigned int aliveEnemies;
	float nextWaveStartsIn; // Seconds until next wave starts

	std::random_device rd;
	std::mt19937 e2;
	std::uniform_real_distribution<> dist;
	std::uniform_int_distribution<> boolDist;

	void GetAliveEnemyCount();
	void StartNextWave();
	void SpawnEnemies();
	float GetRandomOffsetCoord();
	void SetEnemyStats(Skateboard::Entity ent) const;
	float GetRandomZOffset();
public:
	WaveSystem(std::shared_ptr<b2World> world);

	void Init(GameScene* scn) override;
	void HandleInput(Skateboard::TimeManager* time, GameScene* scn) override;
	void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
	void OnImGuiRender() override;
};

