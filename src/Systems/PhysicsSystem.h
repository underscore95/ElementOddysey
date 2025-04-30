#pragma once
#include "BaseSystem.h"
#include "sktbdpch.h"

#include "box2d/box2d.h"

#include "Skateboard/Scene/ECS.h"
#include "Components/PhysicsBodyComponent.h"

class PhysicsSystem2D :         //BOX2D
    public BaseSystem
{
public:
    PhysicsSystem2D(std::shared_ptr<b2World> world) : PhysWorld(world) {};
    
    void Init(GameScene* scn = NULL) override;

    void HandleInput(Skateboard::TimeManager* time, GameScene* scn = NULL) override;
    void RunUpdate(Skateboard::TimeManager* time, GameScene* scn) override;
    void CheckCollisions(GameScene* scn);

    virtual float GetUpdatePriority() const override { return -1000.0f; }

private:
    std::shared_ptr<b2World> PhysWorld;

    const float timeStep = 1.f / 60.f;
    const int32 velocityIterations = 4;
    const int32 positionIterations = 2;
};

