#include "Entities.h"
#include "Components/PhysicsBodyComponent.h"
#include "Enemies/EnemyComponent.h"
#include "Components/HealthComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Animations/SpriteAnimationComponent.h"
#include "Components/DespawnComponent.h"

Skateboard::Entity Entities::InitEnemy(b2World* world, const glm::vec3& position) {
	using namespace Skateboard;

	auto ent = ECS::CreateEntity("enemy");

	auto& transform = ent.GetComponent<TransformComponent>();
	transform.Trans.Translation = position;

	ent.AddComponent<EnemyComponent>();
	
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
	dynamicBox.SetAsBox(0.50f, 0.50f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);
	ent.AddComponent<RigidBody2D>(body);

	// ANIMATIONS
	auto skelly = Skateboard::AssetManager::LoadTexture(L"/app0/assets/textures/SpriteSheet_Skelly.png", "skellington");
	auto& sprite = ent.AddComponent<SpriteComponent>();
	sprite.SetTexture(skelly);

	Sprites::Add(ent);

	//ADDITIONS
	//use these to define a specific region of the spritesheet to use as the sprite component, These are define in UV space 0,1
	//spriteSprComp.SetUvScale({ 1.f / 9.f, 1.f / 4.f });
	//spriteSprComp.SetUvOffset({ 3.f / 9.f , 1.f / 4.f });

	auto& animator = ent.AddComponent<SpriteAnimComponent>();

	SpriteAnimComponent::Track walk;
	walk.FrameIncrement = float2(1, 0); // where should we step
	walk.FrameCount = 9;				// how many frames are there 
	walk.FrameSize = float2(64, 64);	// our frames are 64 by 64 pixels

	// the track starts at these positions in pixels if the texture
	// the main 4 walks we see with skelly

	// Walk Up
	walk.StartOffset = float2(64, 0);
	animator.Tracks.push_back(walk);

	// Walk Left
	// We maintain the same variable "walk" as most parameters are the same, we only want to change the offset position
	walk.StartOffset = float2(64, 64);
	animator.Tracks.push_back(walk);

	// Walk Down
	walk.StartOffset = float2(64, 64 * 2);
	animator.Tracks.push_back(walk);

	// Walk Right
	walk.StartOffset = float2(64, 64 * 3);
	animator.Tracks.push_back(walk);

	//LETS GET CREATIVE it doth not need to stop here, we could increment diagonally and get a lil twirl animation. (green is not a creative color)
	walk.StartOffset = float2(0, 0);
	walk.FrameIncrement = float2(1, 1);
	animator.Tracks.push_back(walk);

	//Here we control the animator component itself

	animator.FrameRate = 10; //FPS
	//tracks the changes of the tracks, when we set this the animation will switch to the new track updating to its frame box size and 
	//NOTE: this doesnt reset the current frame animation is on so the new track will simply continue from the same position 
	//SpriteAnimComponent::CurrentFrame is also exposed to you so you can set a custom start when switching
	animator.NewTrack = 0;

	//restart the animation from frame 0
	animator.Restart = true;

	//if the animation is playing; this will be set to false if looping is false after single completion
	animator.Playing = true;

	//paused is same as playing but will never be unset by anything other than developer
	animator.Paused = false;

	//restarts the animation 
	animator.Loop = true;

	//mirrors the animation but does not change its playback "direction", with skelly if we set this to true on X for example, when walking right, he will face left but walk backwards
	animator.FlipY = false;
	animator.FlipX = false;

	//controls the playback fo the animation, are we incrementing forward or backward
	animator.Reverse = false;

	return ent;
}