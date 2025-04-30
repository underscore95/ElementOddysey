#include "Entities.h"
#include "Components/PhysicsBodyComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Animations/SpriteAnimationComponent.h"
#include "Components/DespawnComponent.h"
#include "Enemies/KnightComponent.h"

Skateboard::Entity Entities::InitKnightEnemy(b2World* world, const glm::vec3& position) {
	using namespace Skateboard;

	auto ent = ECS::CreateEntity("knight");

	auto& transform = ent.GetComponent<TransformComponent>();
	transform.Trans.Translation = position;

	auto& enemy = ent.AddComponent<EnemyComponent>();
	enemy.canAttack = false; // KnightSystem will set this to true when it is attacking
	enemy.rotates = false;
	enemy.souls = 2;

	ent.AddComponent<HealthComponent>(200);
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
	auto knight = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/NightBorne.png", "knight");
	enemy.stats.HealthAddition = 100;
	enemy.stats.DamageScalar = 1.5f;
	enemy.moveSpeed = 1.20f;

	sprite.SetTexture(knight);
	sprite.SetSize(glm::vec2(2.0f, 2.0f));

	Sprites::Add(ent);

	// ANIMATIONS
	auto& animator = ent.AddComponent<SpriteAnimComponent>();

	SpriteAnimComponent::Track run;
	run.StartOffset = glm::vec2(0, 0);
	run.FrameIncrement = glm::vec2(1, 0);
	run.FrameCount = 6;
	run.FrameSize = glm::vec2(80, 80);
	animator.Tracks.push_back(run);

	SpriteAnimComponent::Track attack;
	attack.StartOffset = glm::vec2(0, 80);
	attack.FrameIncrement = glm::vec2(1, 0);
	attack.FrameCount = 12;
	attack.FrameSize = glm::vec2(80, 80);
	animator.Tracks.push_back(attack);

	animator.FrameRate = 8;

	animator.Play(0);


	auto& knightComp = ent.AddComponent<KnightComponent>();
	knightComp.attackDuration = std::chrono::milliseconds(static_cast<int>(1000.0 * attack.FrameCount / animator.FrameRate));
	knightComp.defaultTrack = 0;
	knightComp.attackTrack = 1;

	return ent;
}