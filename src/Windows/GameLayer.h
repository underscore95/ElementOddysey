#pragma once
#include <Skateboard.h>



// include scene hierarchy panel
#include "Panels/SceneHierarchyPanel.h"



class GameLayer final : public Skateboard::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() final override;

	// TODO: Remove this. See comment in base Layer class.
	virtual void OnResize(int newClientWidth, int newClientHeight) final override;

	virtual void OnEvent(Skateboard::Event& e) final override;

	virtual bool OnHandleInput(Skateboard::TimeManager* time) final override;
	virtual bool OnUpdate(Skateboard::TimeManager* time) final override;
	virtual void OnRender() final override;
	virtual void OnImGuiRender() final override;

private:
	// Scene and the hierarchical view
	std::shared_ptr<Skateboard::Scene> p_Scene{ nullptr };
	SceneHierarchyPanel m_SceneHierarchy;

	void ShowLightingTools();

private:
	Skateboard::Light m_Light;
	//Skateboard::Entity m_TestEntity;

	std::shared_ptr<Skateboard::RasterizationPipeline> p_Pipeline;

	std::shared_ptr<Skateboard::RasterizationPipeline> p_WireFramePipeline;

	std::shared_ptr<Skateboard::ComputePipeline> p_ComputePipeline;
	std::shared_ptr<Skateboard::RasterizationPipeline> p_ShadowPassPipeline;
	std::shared_ptr<Skateboard::RaytracingPipeline> p_RaytracingPipeline;
	std::shared_ptr<Skateboard::Texture> m_CrateTexture;
	std::shared_ptr<Skateboard::Texture> m_StoneTexture;
	std::shared_ptr<Skateboard::UnorderedAccessBuffer> m_UAV;
	std::unique_ptr<Skateboard::DescriptorTable> m_TextureDescriptorTable;
	std::unique_ptr<Skateboard::DescriptorTable> m_RasterDescriptorTable;

	//std::shared_ptr<Skateboard::UploadBuffer> m_InstanceBuffer;
	//std::shared_ptr<Skateboard::UploadBuffer> m_TerrainInstancesBuffer;

	std::shared_ptr<Skateboard::FrameBuffer> m_FrameBuffer;
	std::shared_ptr<Skateboard::FrameBuffer> m_WireFrameFrameBuffer;
	std::shared_ptr<Skateboard::FrameBuffer> m_ShadowMap;
	float2 m_ViewPortSize;
	bool m_ScheduleResize;

	float m_FPSClock;
	float m_TestFPS;
	float m_FrameTime;
	bool m_OutputRaytracing;

	Skateboard::PerspectiveCamera m_Camera;
	bool m_ViewPortFocused;


	// Geometry
	uint32_t m_InstanceIndexOffset;
	Skateboard::Entity m_Cube, m_Cube2, m_Terrain, m_Cone, m_Cone2, m_AnalyticSphere;
	uint32_t generalPassCBV, shadowPassCBV, lightCBV, materialSBV, instanceSBV, ProceduralPrimitiveCBV;
};

