#include "PhysicsSystem.h"
#include "Skateboard/Scene/ECS.h"
#include "Skateboard/Scene/Entity.h"
#define SKTBD_LOG_COMPONENT "PHYSICS SYSTEM"
#include "Skateboard/Log.h" 
#include "Components/HealthComponent.h"
#include "GameScene.h"

void PhysicsSystem2D::Init(GameScene* scn)
{

}

void PhysicsSystem2D::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{

}

// TODO Could split up this into a collision system and physics system, and then make phsyics system run on parallel threads?
void PhysicsSystem2D::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	//update physics World
	PhysWorld->Step(timeStep, velocityIterations, positionIterations);

	auto physicsObjects2D = Skateboard::ECS::GetRegistry().view<RigidBody2D, Skateboard::TransformComponent>();

	for (auto entity : physicsObjects2D)
	{
		auto [rigidbody, transform] = physicsObjects2D.get<RigidBody2D, Skateboard::TransformComponent>(entity);

		b2Vec2 translation = rigidbody.Body->GetPosition();
		float  Zrotation = rigidbody.Body->GetAngle();

		transform.Trans.Translation = float3(translation.x, translation.y, transform.Trans.Translation.z);
		transform.Trans.SetEulerZ(Zrotation);
	}

	CheckCollisions(scn);
	//auto Contacts = MainWorld->GetContactList();
}

void PhysicsSystem2D::CheckCollisions(GameScene* scn)
{
	//Get a double linked list of contacts from the World instance.
	b2Contact* contact = PhysWorld.get()->GetContactList();

	//Get how many contacts are happening in the World instance.
	int contactCount = PhysWorld.get()->GetContactCount();

	//Loop through all contacts
	for (int i = 0; i < contactCount; i++)
	{
		//Check if two bodies are touching  in the first contact
		if (contact->IsTouching())
		{
			//Get two touching bodies 
			b2Body* firstBody = contact->GetFixtureA()->GetBody();
			b2Body* secondBody = contact->GetFixtureB()->GetBody();

			//Important: Get the two entities from the touching b2Bodies
			Skateboard::Entity ent1 = (entt::entity)(firstBody->GetUserData().pointer);
			Skateboard::Entity ent2 = (entt::entity)(secondBody->GetUserData().pointer);

			if (Skateboard::ECS::GetRegistry().valid(ent1) && Skateboard::ECS::GetRegistry().valid(ent2)) {
				//Check if the two entities that are touching have the CollisionResponseComponent
				scn->OnCollision(&ent1, &ent2);
			}
		}
		//Get next contact in the from the contact list
		contact = contact->GetNext();
	}
}
