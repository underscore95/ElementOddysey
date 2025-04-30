#include "Entities.h"
#include "Components/HealthComponent.h"
#include "Components/PhysicsBodyComponent.h"

Skateboard::Entity Entities::InitFloor(b2World* world)
{
	using namespace Skateboard;

	//Create an entity and add a Mesh component to it.
	auto floorEnt = ECS::CreateEntity("floor");
	auto& floorMeshComp = floorEnt.AddComponent<StaticMeshComponent>();
	auto& transFloorComp = floorEnt.GetComponent<TransformComponent>();

	// Add collision detection components (this registers to the Physics Ststem)
	floorEnt.AddComponent<HealthComponent>(100);

	floorMeshComp.ModelName = SceneBuilder::CreateDefaultMesh(ShapeType::Cube);
	transFloorComp.Trans.Scale = float3(10, 2, 1);
	b2BodyDef sbdef;
	sbdef.type = b2_staticBody;
	sbdef.position.Set(0, -3);

	sbdef.userData.pointer = (uint64_t)(floorMeshComp.m_entityId);
	auto* body = world->CreateBody(&sbdef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(5.f, 1.0f);		// Notice that this function takes half width and half height which is why it might seem that it's half the size of the box.

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	floorEnt.AddComponent<RigidBody2D>(body);
	return floorEnt;
}
