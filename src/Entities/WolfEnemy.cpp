#include "Entities.h"
#include "Components/PhysicsBodyComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Animations/SpriteAnimationComponent.h"
#include "Components/DespawnComponent.h"
#include "Enemies/JumpComponent.h"

Skateboard::Entity Entities::InitWolfEnemy(b2World* world, const glm::vec3& position) {

	using namespace Skateboard;

	auto ent = ECS::CreateEntity("bat");

	auto& transform = ent.GetComponent<TransformComponent>();
	transform.Trans.Translation = position;

	auto& enemy = ent.AddComponent<EnemyComponent>();
	enemy.stats.DamageAddition = 5;
	enemy.stats.DamageScalar = 1.5;
	enemy.stats.HealthScalar = 3;
	enemy.moveSpeed = 1.5;
	enemy.souls = 5;

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
	dynamicBox.SetAsBox(0.45f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
	ent.AddComponent<RigidBody2D>(body);

	// TEXTURES
	auto& sprite = ent.AddComponent<SpriteComponent>();
	sprite.SetTexture(AssetManager::LoadTexture(L"/app0/assets/textures/wolf.png", "wolf"));
	sprite.SetSize(glm::vec2(1, 2));

	Sprites::Add(ent);

	ent.AddComponent<JumpComponent>();

	Skateboard::AudioRenderer::PlaySFX("wolf_spawn_sfx", false);

	return ent;
}