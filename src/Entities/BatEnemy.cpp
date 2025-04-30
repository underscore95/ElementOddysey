#include "Entities.h"
#include "Components/PhysicsBodyComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Animations/SpriteAnimationComponent.h"
#include "Components/DespawnComponent.h"
#include <random>

std::random_device rd;
std::mt19937 e2(rd());
std::uniform_int_distribution<> dist(0, 6);

Skateboard::Entity Entities::InitBatEnemy(b2World* world, const glm::vec3& position) {
	using namespace Skateboard;

	auto ent = ECS::CreateEntity("bat");

	auto& transform = ent.GetComponent<TransformComponent>();
	transform.Trans.Translation = position;

	auto& enemy = ent.AddComponent<EnemyComponent>();
	enemy.souls = 1;

	ent.AddComponent<HealthComponent>(100);
	ent.AddComponent<DespawnComponent>(std::chrono::seconds(90));

	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set(position.x, position.y);
	bdef.angularVelocity = 0.f;
	bdef.awake = true;
	bdef.userData.pointer = (uint64_t)(transform.m_entityId);

	b2Body* body = world->CreateBody(&bdef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.14f, 0.12f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
	ent.AddComponent<RigidBody2D>(body);

	// TEXTURES
	auto& sprite = ent.AddComponent<SpriteComponent>();
	auto bat = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/bat.png", "bat");
	if (dist(rd) == 0) {
		bat = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/red_bat.png", "red_bat");
		enemy.stats.HealthAddition = 20;
		enemy.moveSpeed = 1.30f;
	}

	sprite.SetTexture(bat);
	sprite.SetSize(glm::vec2(1.0f, 1.0f));

	Sprites::Add(ent);

	// ANIMATIONS
	auto& animator = ent.AddComponent<SpriteAnimComponent>();
	SpriteAnimComponent::Track fly;

	fly.FrameIncrement = glm::vec2(1, 0);
	fly.FrameCount = 3;
	fly.FrameSize = glm::vec2(48, 64);

	animator.Tracks.push_back(fly);
	animator.FrameRate = 3;

	animator.Play(0);

	return ent;
}