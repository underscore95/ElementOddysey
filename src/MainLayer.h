#pragma once

#include <Skateboard.h>
#include "Panels/SceneHierarchyPanel.h"
#include <StateNames.h>
#include "GameScene.h"

class MainLayer final : public Skateboard::Layer
{
public:
	MainLayer();
	virtual ~MainLayer() final override;

	// TODO: Remove this, see comment in base Layer class.
	virtual void OnResize(int newClientWidth, int newClientHeight) final override;

	virtual void OnEvent(Skateboard::Event& e) final override;

	virtual bool OnHandleInput(Skateboard::TimeManager* time) final override;
	virtual bool OnUpdate(Skateboard::TimeManager* time) final override;
	virtual void OnRender() final override;
	virtual void OnImGuiRender() final override;

	void ChangeScene(StateNames newScene);

private:
	// This works with a scene context, hence why you can have multiple scenes in a single layer.
	SceneHierarchyPanel m_SceneHierarchy;

	// TODO: Scene manager perhaps? Or this is the level in which you may provide one. (a scene loader even)
	//std::shared_ptr<Skateboard::Scene> p_Scene;
	std::shared_ptr<GameScene> p_CurrentScene;
	std::shared_ptr<GameScene> mainScene{ nullptr };

	//
	Skateboard::Light m_SunLight;


	Skateboard::PerspectiveCamera m_Camera;
	Skateboard::CameraController m_CameraController;

	float dt = 0;

};