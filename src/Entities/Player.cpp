#include "Entities.h"
#include "Components/HealthComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/PlayerComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Scripts/PlayerControllerScript.h"
#include "Components/BarComponent.h"
#include "Components/ManaComponent.h"

Skateboard::Entity Entities::InitPlayer(b2World* world, UserID user, const Element& element) {
	using namespace Skateboard;

	auto ent = ECS::CreateEntity("player");

	ent.AddNativeScriptComponent<PlayerControllerScript>();

	auto& staticMeshComponent = ent.AddComponent<StaticMeshComponent>();
	AssetManager::LoadModel(L"/app0/assets/models/golem_model.fbx", "player");
	staticMeshComponent.ModelName = "player";

	ent.AddComponent<PlayerComponent>(element, user);

	auto& transform = ent.GetComponent<TransformComponent>();

	auto& health = ent.AddComponent<HealthComponent>(100);

	auto& mana = ent.AddComponent<ManaComponent>(100);

	ent.AddComponent<BarComponent>(0, health.maxHealth, health.GetHealth());

	b2BodyDef bdef;
	bdef.type = b2_kinematicBody;
	bdef.position.Set(rand() % 6 - 3, rand() % 6 - 3);
	bdef.angularVelocity = 0.f;
	bdef.awake = true;
	bdef.userData.pointer = (uint64_t)(transform.m_entityId);

	b2Body* body = world->CreateBody(&bdef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.50f, 0.50f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
	ent.AddComponent<RigidBody2D>(body);

	return ent;
}