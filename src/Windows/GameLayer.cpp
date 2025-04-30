#include "GameLayer.h"
#include "Skateboard/Mathematics.h"
// Temporary
#include "Skateboard/Scene/SceneBuilder.h"

GameLayer::GameLayer() :
	m_CrateTexture(Skateboard::AssetManager::LoadTexture(L"assets/crate.png", "crate")),
	m_StoneTexture(Skateboard::AssetManager::LoadTexture(L"assets/stone.png", "stone")),
	m_UAV(Skateboard::UnorderedAccessBuffer::Create(L"UAV", Skateboard::UnorderedAccessBufferDesc::Init(m_CrateTexture->GetDesc().Width, m_CrateTexture->GetDesc().Height, m_CrateTexture->GetDesc().Depth))),
	m_TextureDescriptorTable(Skateboard::DescriptorTable::Create(Skateboard::DescriptorTableDesc::Init(Skateboard::ShaderDescriptorTableType_CBV_SRV_UAV))),
	m_RasterDescriptorTable(Skateboard::DescriptorTable::Create(Skateboard::DescriptorTableDesc::Init(Skateboard::ShaderDescriptorTableType_CBV_SRV_UAV))),
	m_FrameBuffer(Skateboard::FrameBuffer::Create(L"Main Render Target", Skateboard::FrameBufferDesc::InitAsFullRenderTarget(1280, 720))),
	m_WireFrameFrameBuffer(Skateboard::FrameBuffer::Create(L"Main Render Target", Skateboard::FrameBufferDesc::InitAsFullRenderTarget(1280, 720))),
	m_ShadowMap(Skateboard::FrameBuffer::Create(L"Main Render Target", Skateboard::FrameBufferDesc::InitAsDepthStencilTargetOnly(2048, 2048))),
	m_FPSClock(1.f),
	m_TestFPS(0.f),
	m_FrameTime(0.f),
	m_Camera(.25f * SKTBD_PI, 1280.f / 720.f, .1f, 1000.f, float3(0.f, 0.f, -10.f), float3(0.f, 0.f, -9.f), float3(0.f, 1.f, 0.f)),
	m_ViewPortSize{ 0.f, 0.f },
	m_ScheduleResize(false),
	m_ViewPortFocused(false),
	m_OutputRaytracing(false),
	m_InstanceIndexOffset(0u)
{
	// Set the scene (*this scene*) to the current context.
	p_Scene = std::make_shared<Skateboard::Scene>("Demo Scene");

	m_Cube = Skateboard::SceneBuilder::AddCubeInstance(p_Scene.get());
	m_Cube2 = Skateboard::SceneBuilder::AddCubeInstance(p_Scene.get());
	m_Terrain = Skateboard::SceneBuilder::AddTerrainInstance(p_Scene.get());
	m_Cone = Skateboard::SceneBuilder::AddConeInstance(p_Scene.get());
	m_Cone2 = Skateboard::SceneBuilder::AddConeInstance(p_Scene.get());

	Skateboard::RaytracingAABBDesc aabbDesc = {};
	float analyticSphereRadius = 3.f;
	aabbDesc.Name = L"Analytic Sphere";
	aabbDesc.Type = Skateboard::GeometryType_Procedural_Analytics;
	aabbDesc.BoundingBox.MinX = 0.f - analyticSphereRadius;	// The center will be defined by the instance translation!
	aabbDesc.BoundingBox.MinY = 0.f - analyticSphereRadius;
	aabbDesc.BoundingBox.MinZ = 0.f - analyticSphereRadius;
	aabbDesc.BoundingBox.MaxX = 0.f + analyticSphereRadius;
	aabbDesc.BoundingBox.MaxY = 0.f + analyticSphereRadius;
	aabbDesc.BoundingBox.MaxZ = 0.f + analyticSphereRadius;
	m_AnalyticSphere = Skateboard::SceneBuilder::AddRaytracingProceduralGeometryInstance(p_Scene.get(), aabbDesc);

	generalPassCBV = Skateboard::MemoryManager::CreateConstantBuffer(L"Pass Constant Buffer", 1, sizeof(Skateboard::SKTBDPassBuffer));
	shadowPassCBV = Skateboard::MemoryManager::CreateConstantBuffer(L"Shadow Constant buffer", 1, sizeof(Skateboard::SKTBDPassBuffer));
	lightCBV = Skateboard::MemoryManager::CreateConstantBuffer(L"Light Constant Buffer", 1, sizeof(Skateboard::SKTBDLightBuffer));
	materialSBV = Skateboard::MemoryManager::CreateStructuredBuffer(L"Material Structured Buffer", p_Scene->GetTotalInstanceCount(), sizeof(Skateboard::SKTBDMaterialGPUData));
	ProceduralPrimitiveCBV = Skateboard::MemoryManager::CreateConstantBuffer(L"Procedural Primitive Constant Buffer", 1, sizeof(Skateboard::SKTBDProceduralPrimitiveBuffer));
	p_Scene->BuildInstanceStructuredBuffer(sizeof(Skateboard::SKTBDInstanceBuffer));
	m_SceneHierarchy.SetContext(p_Scene);

	// Setup a light projection matrix for the shadowmap
	m_Light.GenerateOrthographicProjectionMatrix(100.f, 100.f * 1280.f / 720.f, .1f, 1000.f);
	m_Light.GenerateViewMatrix();

	Skateboard::SKTBDPassBuffer pass = {};
	pass.ViewMatrix = m_Camera.GetViewMatrix();
	pass.ProjectionMatrix = m_Camera.GetProjectionMatrix();
	pass.ViewMatrixInverse =  glm::inverse(pass.ViewMatrix);
	pass.ProjectionMatrixInverse = glm::inverse(pass.ProjectionMatrix);
	pass.CameraPosition = m_Camera.GetPosition();
	Skateboard::MemoryManager::UploadData(generalPassCBV, 0, &pass);

	pass.ViewMatrix = m_Light.GetViewMatrix();
	pass.ProjectionMatrix = m_Light.GetProjectionMatrix();
	Skateboard::MemoryManager::UploadData(shadowPassCBV, 0, &pass);

	Skateboard::TransformComponent* transform = &m_Cube2.GetComponent<Skateboard::TransformComponent>();
	transform->Translation = float3(5.f, 1.f, 4.f);
	transform = &m_Terrain.GetComponent<Skateboard::TransformComponent>();
	transform->Scale = float3(10.f, 1.f, 10.f);
	transform->Translation = float3(0.f, -2.f, 0.f);
	transform = &m_Cone.GetComponent<Skateboard::TransformComponent>();
	transform->Translation = float3(-3.f, -1.5f, -2.f);
	transform = &m_Cone2.GetComponent<Skateboard::TransformComponent>();
	transform->Scale = float3(2.f, 2.f, 2.f);
	transform->Translation = float3(0.f, -1.f, 8.f);
	transform = &m_AnalyticSphere.GetComponent<Skateboard::TransformComponent>();
	transform->Translation = float3(-5.f, 4.f, 0.f);

	Skateboard::StaticMeshInstanceComponent* instanceComp = &m_Cube2.GetComponent<Skateboard::StaticMeshInstanceComponent>();
	instanceComp->MaterialId = 1;
	instanceComp = &m_Cube.GetComponent<Skateboard::StaticMeshInstanceComponent>();
	instanceComp->MaterialId = 1;
	instanceComp = &m_Terrain.GetComponent<Skateboard::StaticMeshInstanceComponent>();
	instanceComp->MaterialId = 0;
	instanceComp = &m_Cone.GetComponent<Skateboard::StaticMeshInstanceComponent>();
	instanceComp->MaterialId = 1;
	instanceComp = &m_Cone2.GetComponent<Skateboard::StaticMeshInstanceComponent>();
	instanceComp->MaterialId = 1;
	instanceComp = &m_AnalyticSphere.GetComponent<Skateboard::StaticMeshInstanceComponent>();
	instanceComp->MaterialId = 1;

	// Create the buffers with instancing data
	p_Scene->ResetInstanceBufferForAllEntities();
	Skateboard::SceneBuilder::GenerateGeometryBuffers(p_Scene.get());

	Skateboard::SKTBDLightBuffer lightData
	{
		m_Light.GetDiffuse(),
		m_Light.GetFalloffStart(),
		m_Light.GetDirection(),
		m_Light.GetFalloffEnd(),
		m_Light.GetPosition(),
		m_Light.GetSpotPower(),
		m_Light.GetRadiance(),
		0.f,
		m_Light.GetTextureSpaceMatrix()
	};
	Skateboard::MemoryManager::UploadData(lightCBV, 0, &lightData);

	Skateboard::SKTBDMaterialGPUData materialData
	{
		float4(1.f, 1.f, 0.f, 1.f),
		0,
		int3(0, 0, 0),
		float3(.11f, .22f, .11f),
		0.f,
		float3(0.f, 0.f, 0.f),
		1.f
	};
	Skateboard::MemoryManager::UploadData(materialSBV, 0, &materialData);
	materialData.m_AlbedoMapIndex = 1;
	Skateboard::MemoryManager::UploadData(materialSBV, 1, &materialData);

	Skateboard::SKTBDProceduralPrimitiveBuffer proceduralData = {};
	proceduralData.Radius = analyticSphereRadius;
	Skateboard::MemoryManager::UploadData(ProceduralPrimitiveCBV, 0, &proceduralData);

	// Register the textures into our descriptor table
	m_TextureDescriptorTable->AddDescriptor(Skateboard::Descriptor::ShaderResourceView(m_CrateTexture.get(), 0u, 2u));
	m_TextureDescriptorTable->AddDescriptor(Skateboard::Descriptor::ShaderResourceView(m_StoneTexture.get(), 1u, 2u));
	m_TextureDescriptorTable->GenerateTable();

	m_RasterDescriptorTable->AddDescriptor(Skateboard::Descriptor::ConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(lightCBV), 3, 0));
	m_RasterDescriptorTable->AddDescriptor(Skateboard::Descriptor::ShaderResourceView(Skateboard::MemoryManager::GetUploadBuffer(materialSBV), 3, 0));
	m_RasterDescriptorTable->AddDescriptor(Skateboard::Descriptor::ShaderResourceView(m_ShadowMap.get(), 4, 0));
	m_RasterDescriptorTable->GenerateTable();

	Skateboard::RasterizationPipelineDesc rasterDesc = {};
	rasterDesc.SetType(Skateboard::RasterizationPipelineType_Default);
	rasterDesc.SetInputLayout(Skateboard::SceneBuilder::GetVertexLayout());
	rasterDesc.SetWireFrame(false);
	rasterDesc.SetDepthBias(PIPELINE_SETTINGS_DEFAULT_DEPTH_BIAS);
	rasterDesc.SetVertexShader(L"vertexshader_vs.cso", L"main");
	rasterDesc.SetPixelShader(L"pixelshader_ps.cso", L"main");
	//desc.SetGeometryShader(L"geometryshader_gs.cso", L"main");
	//desc.SetHullShader(L"hullshader_hs.cso", L"main");
	//desc.SetDomainShader(L"domainshader_ds.cso", L"main");
	rasterDesc.AddRootConstant(&m_InstanceIndexOffset, 1, 0, 1, Skateboard::ShaderVisibility_VertexShader);// Define a root constant that will be used to index the instance buffer
	rasterDesc.AddConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(generalPassCBV), 0);
	rasterDesc.AddShaderResourceView(p_Scene->GetInstanceStructuredBuffer(), 0);
	//desc.AddShaderResourceView(Skateboard::SceneBuilder::Singleton().GetVertexBuffer()->GetBuffer(), 1);
	//rasterDesc.AddShaderResourceView(m_CrateTexture.get(), 2);
	rasterDesc.AddDescriptorTable(m_TextureDescriptorTable.get(), Skateboard::ShaderVisibility_PixelShader);
	rasterDesc.AddDescriptorTable(m_RasterDescriptorTable.get(), Skateboard::ShaderVisibility_PixelShader);
	//rasterDesc.AddConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(lightCBV), 3, 0, Skateboard::ShaderVisibility_PixelShader);
	//rasterDesc.AddShaderResourceView(Skateboard::MemoryManager::GetUploadBuffer(materialSBV), 3, 0, Skateboard::ShaderVisibility_PixelShader);
	//rasterDesc.AddShaderResourceView(m_ShadowMap.get(), 4);
	rasterDesc.AddSampler(Skateboard::SamplerDesc::InitAsDefaultTextureSampler(0));
	rasterDesc.AddSampler(Skateboard::SamplerDesc::InitAsDefaultShadowSampler(1));
	p_Pipeline.reset(Skateboard::RasterizationPipeline::Create(L"Graphics Pipeline", rasterDesc));
	rasterDesc.SetWireFrame(true);
	p_WireFramePipeline.reset(Skateboard::RasterizationPipeline::Create(L"Wireframe Graphics Pipeline", rasterDesc));

	rasterDesc = {};
	rasterDesc.SetType(Skateboard::RasterizationPipelineType_DepthOnly);
	rasterDesc.SetInputLayout(Skateboard::SceneBuilder::GetVertexLayout());
	rasterDesc.SetWireFrame(false);
	rasterDesc.SetDepthBias(10000);
	rasterDesc.SetVertexShader(L"vertexshader_vs.cso", L"main");
	rasterDesc.AddRootConstant(&m_InstanceIndexOffset, 1, 0, 1, Skateboard::ShaderVisibility_VertexShader);// Define a root constant that will be used to index the instance buffer
	rasterDesc.AddConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(shadowPassCBV), 0);
	rasterDesc.AddShaderResourceView(p_Scene->GetInstanceStructuredBuffer(), 0);
	p_ShadowPassPipeline.reset(Skateboard::RasterizationPipeline::Create(L"ShadowMap Pipeline", rasterDesc));

	Skateboard::ComputePipelineDesc computeDesc = {};
	computeDesc.SetDispatchSize((uint32_t)m_CrateTexture->GetDesc().Width / 16, m_CrateTexture->GetDesc().Height / 16, 1);
	computeDesc.SetComputeShader(L"compute_cs.cso", L"main");
	computeDesc.AddShaderResourceView(m_CrateTexture.get(), 0);
	computeDesc.AddUnorderedAccessView(m_UAV.get(), 0);
	p_ComputePipeline.reset(Skateboard::ComputePipeline::Create(L"Compute Pipeline", computeDesc));

	struct RaytracingPayload
	{
		float4 ColourAndDistance;
		uint32_t RecursionDepth;
	};

	struct ProceduralAttributes
	{
		float3 Normal;
		float2 Uv;
	};

	Skateboard::RaytracingPipelineDesc raytracingDesc = {};
	raytracingDesc.SetDispatchSize(1280, 720, 1u); // Unnecessary when resizing with buffers anyways..
	raytracingDesc.SetRaytracingLibrary(L"Raytracing_lib.cso", L"RayGenShader");
	raytracingDesc.AddHitGroup(L"HitGroup", nullptr, L"ClosestHitShader", nullptr, Skateboard::RaytracingHitGroupType_Triangles);
	raytracingDesc.AddHitGroup(L"ShadowHitGroup", L"ShadowAnyHitShader", L"ShadowClosestHitShader", nullptr, Skateboard::RaytracingHitGroupType_Triangles);
	raytracingDesc.AddHitGroup(L"ProceduralHitGroup", nullptr, L"ProceduralClosestHitShader", L"ProceduralIntersectionShader", Skateboard::RaytracingHitGroupType_Procedural);
	raytracingDesc.AddHitGroup(L"ProceduralShadowHitGroup", L"ShadowAnyHitShader", L"ShadowClosestHitShader", L"ProceduralIntersectionShader", Skateboard::RaytracingHitGroupType_Procedural);
	raytracingDesc.AddMissShader(L"MissShader");
	raytracingDesc.AddMissShader(L"ShadowMissShader");
	raytracingDesc.AddCallableShader(L"CallableShader");
	raytracingDesc.SetConfig(sizeof(RaytracingPayload), sizeof(ProceduralAttributes), 2u);
	raytracingDesc.AddShaderResourceView(p_Scene->GetTopLevelAccelerationStructure(), 0, 1, Skateboard::RaytracingShaderVisibility_Global);
	raytracingDesc.AddConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(generalPassCBV), 0, 1, Skateboard::RaytracingShaderVisibility_Global);
	raytracingDesc.AddShaderResourceView(p_Scene->GetVertexBuffer()->GetBuffer(), 0, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddShaderResourceView(p_Scene->GetIndexBuffer()->GetBuffer(), 1, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddShaderResourceView(p_Scene->GetVertexOffsetsBuffer(), 2, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddShaderResourceView(p_Scene->GetIndexOffsetsBuffer(), 3, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddShaderResourceView(Skateboard::MemoryManager::GetUploadBuffer(materialSBV), 4, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddDescriptorTable(m_TextureDescriptorTable.get(), Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddShaderResourceView(p_Scene->GetInstanceStructuredBuffer(), 5, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(lightCBV), 1, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddConstantBufferView(Skateboard::MemoryManager::GetUploadBuffer(ProceduralPrimitiveCBV), 2, 0, Skateboard::RaytracingShaderVisibility_Local_Hitgroup);
	raytracingDesc.AddSampler(Skateboard::SamplerDesc::InitAsDefaultTextureSampler(0, 0, Skateboard::RaytracingShaderVisibility_Global));
	p_RaytracingPipeline.reset(Skateboard::RaytracingPipeline::Create(L"Raytracing Pipeline", raytracingDesc));
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnResize(int newClientWidth, int newClientHeight)
{
	// This is called when the windows' window resizes, not the imgui viewports!
}

void GameLayer::OnEvent(Skateboard::Event& e)
{



}

bool GameLayer::OnHandleInput(Skateboard::TimeManager* time)
{
	// Handle the camera inputs
	// This is handled by the platform inputs as different platforms will control the camera with different input systems
	// If any movement is detected, simply upload the new data to the GPU
	// Typically, the engine should not even have camera movement coded in it as this would be up to the game scripts
	if (m_ViewPortFocused)
		Skateboard::Input::ControlCamera(m_Camera, time->DeltaTime());
	if (m_Camera.HasMoved())
	{
		Skateboard::SKTBDPassBuffer pass = {};
		pass.ViewMatrix = m_Camera.GetViewMatrix();
		pass.ProjectionMatrix = m_Camera.GetProjectionMatrix();
		pass.ViewMatrixInverse = glm::inverse(pass.ViewMatrix);
		pass.ProjectionMatrixInverse = glm::inverse(pass.ProjectionMatrix);
		pass.CameraPosition = m_Camera.GetPosition();
		Skateboard::MemoryManager::UploadData(generalPassCBV, 0, &pass);

		// Once camera inputs are handled, make sure to deactivate movement until new inputs are received
		m_Camera.SetMoved(false);
	}
	return true;
}

bool GameLayer::OnUpdate(Skateboard::TimeManager* time)
{
	float dt = time->DeltaTime();
	m_FPSClock += dt;
	if (m_FPSClock > 1.f)
	{
		m_FPSClock = 0.f;
		m_TestFPS = 1.f / dt;
		m_FrameTime = dt * 1000.f;
	}

	// Update scene internal data!
	p_Scene->OnUpdate(time);

	return true;
}

void GameLayer::OnRender()
{
	if (m_ScheduleResize)
	{
		// This could be handled better with an application event queue that is process
		SKTBD_APP_INFO("Resizing frame buffers, one GPU flush expected..");
		Skateboard::RenderCommand::WaitUntilIdle();
		m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		p_RaytracingPipeline->ResizeDispatchAndOutputUAV((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_WireFrameFrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_Camera.OnResize((int)m_ViewPortSize.x, (int)m_ViewPortSize.y);
		m_Camera.SetMoved(true);
		m_ScheduleResize = false;
	}

	Skateboard::RenderCommand::BeginScene(p_Scene.get());
	if (!m_OutputRaytracing)
	{
		p_Scene->BindGeometryBuffers();
		// Shadow pass
		m_ShadowMap->Bind();
		Skateboard::RenderCommand::DrawIndexedInstanced(p_ShadowPassPipeline.get(), p_Scene->GetMeshID("Cone"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_ShadowPassPipeline.get(), p_Scene->GetMeshID("Cube"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_ShadowPassPipeline.get(), p_Scene->GetMeshID("CubeSphere"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_ShadowPassPipeline.get(), p_Scene->GetMeshID("Cylinder"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_ShadowPassPipeline.get(), p_Scene->GetMeshID("Sphere"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_ShadowPassPipeline.get(), p_Scene->GetMeshID("Terrain"), &m_InstanceIndexOffset);
		m_ShadowMap->Unbind();

		//// Compute pass
		//m_UAV->TransitionToType(Skateboard::GPUResourceType_UnorderedAccessBuffer);
		//Skateboard::RenderInstruction::Dispatch(p_ComputePipeline.get());
		//m_UAV->TransitionToType(Skateboard::GPUResourceType_Texture2D);

		// Raster pass
		m_FrameBuffer->Bind();
		Skateboard::RenderCommand::DrawIndexedInstanced(p_Pipeline.get(), p_Scene->GetMeshID("Cone"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_Pipeline.get(), p_Scene->GetMeshID("Cube"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_Pipeline.get(), p_Scene->GetMeshID("CubeSphere"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_Pipeline.get(), p_Scene->GetMeshID("Cylinder"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_Pipeline.get(), p_Scene->GetMeshID("Sphere"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_Pipeline.get(), p_Scene->GetMeshID("Terrain"), &m_InstanceIndexOffset);
		m_FrameBuffer->Unbind();

		// Debug pass
		m_WireFrameFrameBuffer->Bind();
		Skateboard::RenderCommand::DrawIndexedInstanced(p_WireFramePipeline.get(), p_Scene->GetMeshID("Cone"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_WireFramePipeline.get(), p_Scene->GetMeshID("Cube"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_WireFramePipeline.get(), p_Scene->GetMeshID("CubeSphere"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_WireFramePipeline.get(), p_Scene->GetMeshID("Cylinder"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_WireFramePipeline.get(), p_Scene->GetMeshID("Sphere"), &m_InstanceIndexOffset);
		Skateboard::RenderCommand::DrawIndexedInstanced(p_WireFramePipeline.get(), p_Scene->GetMeshID("Terrain"), &m_InstanceIndexOffset);
		m_WireFrameFrameBuffer->Unbind();
	}
	else
	{
		Skateboard::RenderCommand::DispatchRays(p_RaytracingPipeline.get());
		Skateboard::RenderCommand::CopyUAVToFrameBuffer(m_FrameBuffer.get(), p_RaytracingPipeline->GetOutputUAV());
	}

	Skateboard::RenderCommand::EndScene();
}

void GameLayer::OnImGuiRender()
{
	//ImGui::ShowDemoWindow();

	ImGui::SetNextWindowPos({ 1097,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 183,180 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Test Window");
	ImGui::Text("FPS: %f", m_TestFPS);
	ImGui::Text("Frame Time [ms]: %f", m_FrameTime);

	static std::array<std::string, 2u> gui_RenderingModes = { "Rasterization", "Raytracing" };
	static uint32_t gui_SelectedRenderingMode = 0u;
	if (ImGui::BeginCombo("Renderer", gui_RenderingModes[gui_SelectedRenderingMode].c_str()))
	{
		for (uint32_t j = 0u; j < gui_RenderingModes.size(); ++j)
		{
			if ((j > 0u && !Skateboard::GraphicsContext::Context->IsRaytracingSupported())) break;

			const bool selected = j == gui_SelectedRenderingMode;
			if (ImGui::Selectable(gui_RenderingModes[j].c_str(), selected))
			{
				gui_SelectedRenderingMode = j;
				m_OutputRaytracing = j == 1u;
			}
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	
	ImGui::Separator();
	float3 pos = m_Camera.GetPosition();
	float3 rot = m_Camera.GetRotation();
	if (ImGui::DragFloat3("Camera Pos", &pos.x, .1f))
	{
		m_Camera.SetPosition(pos);
		m_Camera.UpdateViewMatrix();
		m_Camera.SetMoved(true);
	}
	if (ImGui::DragFloat3("Camera Rot", &rot.x, .1f))
	{
		m_Camera.SetRotation(rot);
		m_Camera.UpdateViewMatrix();
		m_Camera.SetMoved(true);
	}
	ImGui::End();
	// Render light tools
	ShowLightingTools();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	ImGui::SetNextWindowPos({ 268,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 646,720 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("ViewPort");
	m_ViewPortFocused = ImGui::IsWindowHovered() || Skateboard::Input::IsMouseActive();
	const ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))	// This will avoid continous resizing while dragging the resize bars. I did not find a better way..
	{
		const bool resized = viewPortSize.x && viewPortSize.y && (m_ViewPortSize.x != viewPortSize.x || m_ViewPortSize.y != viewPortSize.y);
		if (resized)
		{
			m_ViewPortSize = { viewPortSize.x, viewPortSize.y };
			m_ScheduleResize = true;
		}
	}
	ImGui::Image(m_FrameBuffer->GetRenderTargetAsImGuiTextureID(), viewPortSize);
	ImGui::End();
	ImGui::PopStyleVar();


	ImGui::SetNextWindowPos({ 916,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 179,720 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Debug Buffers");
	ImGui::Text("Wireframe View");
	ImGui::Image(m_WireFrameFrameBuffer->GetRenderTargetAsImGuiTextureID(), { 240.f * m_WireFrameFrameBuffer->GetAspectRatio(), 240 });
	ImGui::Separator();
	ImGui::Text("Depth Buffer");
	ImGui::Image(m_FrameBuffer->GetDepthStencilTargetAsImGuiTextureID(), { 240.f * m_FrameBuffer->GetAspectRatio(), 240 });
	ImGui::Separator();
	ImGui::Text("Shadow Buffer");
	ImGui::Image(m_ShadowMap->GetDepthStencilTargetAsImGuiTextureID(), { 240.f * m_ShadowMap->GetAspectRatio(), 240 });
	ImGui::Separator();

	// Render the current entity's components (*if one has been selected!*)
	m_SceneHierarchy.OnImGuiRender();

	ImGui::End();
}

void GameLayer::ShowLightingTools()
{
	// Light controls
	ImGui::SetNextWindowPos({ 1097,182 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 183,256 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Lights Panel");
	ImGui::Separator();
	ImGui::Text("Scene Light");
	float3	diffuse = m_Light.GetDiffuse();
	float	falloffStart = m_Light.GetFalloffStart();
	float3	direction = m_Light.GetDirection();
	float	falloffEnd = m_Light.GetFalloffEnd();
	float3	position = m_Light.GetPosition();
	float	spotPower = m_Light.GetSpotPower();
	float3	radiance = m_Light.GetRadiance();
	bool lightModified = false;

	lightModified |= ImGui::DragFloat3("Diffuse", &diffuse.x, .1f, 0.f, 1.f);
	lightModified |= ImGui::DragFloat3("Radiance", &radiance.x, .1f, 0.f, 1.f);
	lightModified |= ImGui::DragFloat("Falloff Start", &falloffStart, .1f);
	lightModified |= ImGui::DragFloat("Falloff End", &falloffEnd, .1f);
	lightModified |= ImGui::DragFloat("Spot Power", &spotPower, .1f);
	lightModified |= ImGui::DragFloat3("Position", &position.x, .1f);
	lightModified |= ImGui::DragFloat3("Direction", &direction.x, .1f, -1.f, 1.f);

	if (lightModified)
	{
		m_Light.SetDiffuse(diffuse);
		m_Light.SetRadiance(radiance);
		m_Light.SetFalloffStart(falloffStart);
		m_Light.SetFalloffEnd(falloffEnd);
		m_Light.SetPosition(position);
		m_Light.SetDirection(direction);
		m_Light.SetSpotPower(spotPower);

		m_Light.GenerateViewMatrix();

		Skateboard::SKTBDLightBuffer lightData
		{
			m_Light.GetDiffuse(),
			m_Light.GetFalloffStart(),
			m_Light.GetDirection(),
			m_Light.GetFalloffEnd(),
			m_Light.GetPosition(),
			m_Light.GetSpotPower(),
			m_Light.GetRadiance(),
			0.f,
			m_Light.GetTextureSpaceMatrix()
		};
		Skateboard::MemoryManager::UploadData(lightCBV, 0, &lightData);

		Skateboard::SKTBDPassBuffer shadowPass = { m_Light.GetViewMatrix(), m_Light.GetProjectionMatrix() };
		Skateboard::MemoryManager::UploadData(shadowPassCBV, 0, &shadowPass);
	}

	ImGui::End();
}
