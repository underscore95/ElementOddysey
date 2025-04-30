#include "WaveSystem.h"
#include <Skateboard/Log.h>
#include "Entities.h"
#include "Components/PlayerComponent.h"
#include <Skateboard/Scene/Components.h>
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"
#include <algorithm>

void WaveSystem::GetAliveEnemyCount()
{
	auto enemies = Skateboard::ECS::GetRegistry().view<EnemyComponent>();
	aliveEnemies = enemies.size();
}

void WaveSystem::StartNextWave()
{
	++wave;
	nextWaveStartsIn = WAVE_DURATION;
	SpawnEnemies();
}

void WaveSystem::SpawnEnemies()
{
	auto players = Skateboard::ECS::GetRegistry().view<PlayerComponent, Skateboard::TransformComponent>();

	for (auto& ent : players) {
		auto& transform = players.get<Skateboard::TransformComponent>(ent);

		// ENEMIES
		unsigned int bats = std::min(1 + wave, (unsigned int)15);
		for (int i = 0; i < bats; ++i) {
			glm::vec3 randomPosition = glm::vec3(GetRandomOffsetCoord(), GetRandomOffsetCoord(), GetRandomZOffset()) + transform.Trans.Translation;
			SetEnemyStats(Entities::InitBatEnemy(world.get(), randomPosition));
		}

		if (wave > 3) {
			for (int i = 0; i <= wave / 5; ++i) {
				glm::vec3 randomPosition = glm::vec3(GetRandomOffsetCoord(), GetRandomOffsetCoord(), GetRandomZOffset()) + transform.Trans.Translation;
				SetEnemyStats(Entities::InitWolfEnemy(world.get(), randomPosition));
			}
		}

		if (wave > 1) {
			glm::vec3 randomPosition = glm::vec3(GetRandomOffsetCoord(), GetRandomOffsetCoord(), 0);
			SetEnemyStats(Entities::InitKnightEnemy(world.get(), randomPosition));
		}
		// END ENEMIES
	}
}

float WaveSystem::GetRandomOffsetCoord()
{
	return boolDist(e2) ? dist(e2) : -dist(e2);
}

void WaveSystem::SetEnemyStats(Skateboard::Entity ent) const
{
	auto& enemy = ent.GetComponent<EnemyComponent>();
	auto& health = ent.GetComponent<HealthComponent>();

	const float baseHealth = 100 + wave * 5;
	health.maxHealth = baseHealth * enemy.stats.HealthScalar + enemy.stats.HealthAddition;
	health.Heal();

	const float baseDamage = 10 + wave / 2.0f;
	enemy.damage = baseDamage * enemy.stats.DamageScalar + enemy.stats.DamageAddition;
}

float WaveSystem::GetRandomZOffset()
{
	return ((float)rand()) / ((float)INT_MAX); // 0 to 1
}

WaveSystem::WaveSystem(std::shared_ptr<b2World> world) : world{ world }, aliveEnemies{ 0 }, nextWaveStartsIn{ 0 }, e2(rd()), dist(5, 15), boolDist(0, 2)
{
}

void WaveSystem::Init(GameScene* scn)
{
	// If you remove these two lines then the game doesn't game anymore :( (0 sense)
	glm::vec3 randomPosition = glm::vec3(GetRandomOffsetCoord(), GetRandomOffsetCoord(), 0);
	SetEnemyStats(Entities::InitKnightEnemy(world.get(), randomPosition));
}

void WaveSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void WaveSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	GetAliveEnemyCount();
	if (aliveEnemies < MAX_ENEMIES) {
		nextWaveStartsIn -= time->DeltaTime();
	}

	if (nextWaveStartsIn <= 0 || aliveEnemies <= 0) {
		StartNextWave();
	}
}

void WaveSystem::OnImGuiRender()
{
	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_35"));

	std::string str("Alive Enemies: " + std::to_string(aliveEnemies) + " / " + std::to_string(MAX_ENEMIES));
	ImGui::TextColored(ImVec4(1, 1, 1, 1), str.c_str());
	std::string str2("Next Wave In: " + std::to_string((int)nextWaveStartsIn));
	if (aliveEnemies >= MAX_ENEMIES) {
		str2 += " (PAUSED - Too many enemies!)";
	}
	ImGui::TextColored(ImVec4(1, 1, 1, 1), str2.c_str());

	ImGui::PopFont();
}
