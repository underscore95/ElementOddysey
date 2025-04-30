#include "Entities.h"
#include "Components/HealthComponent.h"
#include "Components/PhysicsBodyComponent.h"

Skateboard::Entity Entities::InitCamera() {
	using namespace Skateboard;
	
	// Creating a camera entity
	auto cameraEnt = ECS::CreateEntity("camera");
	auto& cameraComp = cameraEnt.AddComponent<CameraComponent>();
	auto& camTrans = cameraEnt.GetComponent<TransformComponent>().Trans;
	camTrans.Translation = float3(0, 0, -10);
	camTrans.LookAt(float3(0, 0, 0));
	//cameraComp.Camera = std::make_unique<PerspectiveCamera>(/*FOV*/.25f * SKTBD_PI, /*Aspect Ratio*/ 1280.f / 720.f,/*Near plane*/ .1f,/*FarPlane*/ 1000.f);
	cameraComp.Camera = std::make_unique<OrthographicCamera>(/*View Width*/7.5f * (1280.f / 720.f), /*view height*/ 7.5f,/*Near plane*/ .1f,/*FarPlane*/ 1000.f);
	return cameraEnt;
}