#include "Entities.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/WeakAttackComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Animations/SpriteAnimationComponent.h"
#include "Components/DespawnComponent.h"
#include "Components/HealthComponent.h"
#include "Components/PlayerComponent.h"

Skateboard::Entity Entities::InitWeakAttack(b2World* world, const glm::float3& position, const glm::float2& velocity, float damage, Element element)
{

	using namespace Skateboard;
	auto ent = ECS::CreateEntity("weakattack");

	auto& transform = ent.GetComponent<TransformComponent>();
	transform.Trans.Translation = position;

	auto& a = ent.AddComponent<WeakAttackComponent>(b2Vec2(velocity.x, velocity.y), damage);

	ent.AddComponent<DespawnComponent>(std::chrono::seconds(10));
	ent.AddComponent<HealthComponent>(1);

	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.Set(position.x, position.y);
	bdef.angularVelocity = 0.f;
	bdef.awake = true;
	bdef.userData.pointer = (uint64_t)(transform.m_entityId);

	b2Body* body = world->CreateBody(&bdef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.25f, 0.25f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.isSensor = true;

	body->CreateFixture(&fixtureDef);
	ent.AddComponent<RigidBody2D>(body);
	
	auto& sprite = ent.AddComponent<SpriteComponent>();
	
	if (element == Element::FIRE) {
		auto fireattack = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/fireball.png", "fireattack");
		sprite.SetTexture("fireattack");
	}
	else if (element == Element::WATER) {
		auto waterattack = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/WaterSprite.png", "waterattack");
		sprite.SetTexture("waterattack");
	}
	else if (element == Element::WIND) {
		auto windattack = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/WindSprite.png", "windattack");
		sprite.SetTexture("windattack");
	}
	else {
		auto darkattack = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/DarkSprite.png", "darkattack");
		sprite.SetTexture("darkattack");
	}

	sprite.SetSize(glm::vec2(0.5, 0.5));

	Sprites::Add(ent);

	return ent;
}
