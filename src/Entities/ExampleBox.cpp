#include "Entities.h"
#include "Components/HealthComponent.h"
#include "Components/PhysicsBodyComponent.h"

Skateboard::Entity Entities::InitBox(b2World* world)
{
	using namespace Skateboard;

	//Create an entity and add a mesh component to it.
	auto boxEnt = ECS::CreateEntity("Mesh");
	auto& staticMeshComponent = boxEnt.AddComponent<StaticMeshComponent>();
	auto& transformComponent = boxEnt.AddComponent<TransformComponent>();
	transformComponent.Trans.Translation = glm::vec3(0, 4, 0);
	// This creates a model and stores it in the asset manager. Here we only get a Key String to the model to access it cleanly.
	staticMeshComponent.ModelName = SceneBuilder::CreateDefaultMesh(ShapeType::Cube);
	// Change the texture of the model obtained through the AssetManager
	AssetManager::GetModel(staticMeshComponent.ModelName)->SetTexture(AssetManager::LoadTexture(L"assets/crate.png", "crate"));

	// Add collision detection components (this registers to the Physics Ststem)
	boxEnt.AddComponent<HealthComponent>(100);

	//create body
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position.SetZero();
	bdef.angularVelocity = 5.f;
	bdef.awake = true;
	bdef.userData.pointer = (uint64_t)(transformComponent.m_entityId);

	b2Body* body = world->CreateBody(&bdef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.50f, 0.50f);		// Notice that this function takes half width and half height which is why it might seem that it's half the size of the box.

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
	boxEnt.AddComponent<RigidBody2D>(body);
	return boxEnt;
}
