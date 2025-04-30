#include "SceneHierarchyPanel.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
//#include "Skateboard/Renderer/FrameResources.h"
//#include "../GameLayer.h"



SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Skateboard::Scene>& context)
	:
	m_ActiveScene(context)
{
}

void SceneHierarchyPanel::SetContext(const std::shared_ptr<Skateboard::Scene>& context)
{
	m_ActiveScene = context;
	m_SelectionContext = {};
}

void SceneHierarchyPanel::OnImGuiRender()
{
	//Render the scene hierarchy panel.
	ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 266,311 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Scene Hierarchy");

	//For each entity in the scene, draw a node in the panel.
	m_ActiveScene->GetRegistry().each([&](auto entity_id)
		{
			Skateboard::Entity entity{ entity_id, m_ActiveScene.get() };
			DrawEntityNode(entity);
	});

	//Clear the selected context if we click an empty space.
	if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
	{
		m_SelectionContext = {};
	}


	//Pop up menu when right-click on a blank space.
	if (ImGui::BeginPopupContextWindow("Create", 1))
	{
		if (ImGui::MenuItem("Create an empty entity"))
		{
			m_ActiveScene->CreateEntity("Empty Entity");
		}

		ImGui::EndPopup();
	}

	ImGui::End();

	//Render the entity properties panel.
	ImGui::SetNextWindowPos({ 0,313 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 266,407 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Properties");
	if (m_SelectionContext)
	{
		/// <summary>
		/// Render all the components for the currently selected entity.
		/// Additionally render buttons and menus to add/remove components.
		/// </summary>

		DrawComponents(m_SelectionContext);
	}
	ImGui::End();
}


void SceneHierarchyPanel::DrawEntityNode(Skateboard::Entity entity)
{
	auto& entityTag = entity.GetComponent<Skateboard::TagComponent>().tag;


	ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	//This node is a tree which can be expanded in the panel.
	bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityTag.c_str());

	//If *this* node is clicked, set it to the current selected context.
	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = entity;
	}

	//If right clicked, create a popup menu. 
	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
		{
			entityDeleted = true;
		}

		ImGui::EndPopup();
	}

	if (expanded)
	{
		ImGui::TreePop();
	}

	//TODO: Must be a better way... for now keep current selection dirty to update transform.
	//m_SelectionContext.MarkDirty();

	//Deffered deletion to avoid any issues.
	if (entityDeleted)
	{
		//Remove the entity from registery.
		m_ActiveScene->DestroyEntity(entity);
		if (m_SelectionContext == entity)
		{
			//Clear the selected context.
			m_SelectionContext = {};
		}
	}
}

template <typename T, typename UIFunction> bool SceneHierarchyPanel::DrawComponent(const std::string& name, Skateboard::Entity entity, UIFunction uiFunction)
{
	// Return value
	bool componentChanged = false;

	//Set the tree node flags.
	const ImGuiTreeNodeFlags treeNodeFlags =
		ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_AllowItemOverlap |
		ImGuiTreeNodeFlags_Framed |
		ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_FramePadding;

	const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

	if (entity.HasComponent<T>())
	{
		/*auto& component = entity.GetComponent<T>();

		const ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		ImGui::Separator();
		/bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				removeComponent = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			componentChanged = uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			entity.RemoveComponent<T>();
			ResetInstanceBufferForAllEntities();
		}*/
	}

	return componentChanged;
}

static bool DrawVec3Control(const std::string& label, float3& values, float resetValue = 0.f, float columnWidth = 100.f)
{
	// Return value
	bool dataModified = false;

	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	//Two widgets adjacent to each other. i.e (	Text() | DragFloat() )
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);

	ImGui::Text(label.c_str());

	//Evrything below affects the adjacent widget.
	ImGui::NextColumn();


	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

	const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values.x = resetValue;
		dataModified = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	dataModified |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values.y = resetValue;
		dataModified = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	dataModified |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		values.z = resetValue;
		dataModified = true;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	dataModified |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	//Pop the style parameters.
	ImGui::PopStyleVar();
	ImGui::Columns(1);

	//Pop the unique ID for this block of parameters.
	ImGui::PopID();

	return dataModified;
}

void SceneHierarchyPanel::DrawComponents(Skateboard::Entity entity)
{
	//Draw each component linked to *this* entity. 


	if (entity.HasComponent<Skateboard::TagComponent>())
	{
		auto& tag = entity.GetComponent<Skateboard::TagComponent>().tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);


	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Camera"))
		{
			m_SelectionContext.AddComponent<Skateboard::CameraComponent>();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Material"))
		{
			m_SelectionContext.AddComponent<Skateboard::MaterialComponent>();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::BeginMenu("Static Mesh Instance"))
		{
			Skateboard::MeshID meshID = MESHID_ERROR;
			if (ImGui::MenuItem("Cone Mesh"))
			{
				meshID = m_ActiveScene->GetMeshID("Cone");
			}
			if (ImGui::MenuItem("Cube Mesh"))
			{
				meshID = m_ActiveScene->GetMeshID("Cube");
			}
			if (ImGui::MenuItem("CubeSphere Mesh"))
			{
				meshID = m_ActiveScene->GetMeshID("CubeSphere");
			}
			if (ImGui::MenuItem("Cylinder Mesh"))
			{
				meshID = m_ActiveScene->GetMeshID("Cylinder");
			}
			if (ImGui::MenuItem("Sphere Mesh"))
			{
				meshID = m_ActiveScene->GetMeshID("Sphere");
			}
			if (ImGui::MenuItem("Terrain Mesh"))
			{
				meshID = m_ActiveScene->GetMeshID("Terrain");
			}
			if (meshID != MESHID_ERROR)
			{
				m_SelectionContext.AddComponent<Skateboard::StaticMeshInstanceComponent>(meshID);
				m_ActiveScene->ResetInstanceBufferForAllEntities();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	//Draw components.

	if (DrawComponent<Skateboard::TransformComponent>("Transform", entity, [](auto& component) -> bool
		{
			bool modified = false;
			modified |= DrawVec3Control("Translation", component.Translation);
			modified |= DrawVec3Control("Rotation", component.Rotation);
			modified |= DrawVec3Control("Scale", component.Scale, 1.0f);
			return modified;
		}))
	{
		// When the component changed we need to upload the need data to this entity instance
		Skateboard::TransformComponent& component = entity.GetComponent<Skateboard::TransformComponent>();

		Skateboard::SKTBDInstanceBuffer buff = {
			glm::translate(component.Translation)*
			glm::yawPitchRoll(glm::radians(component.Rotation.y), glm::radians(component.Rotation.x), glm::radians(component.Rotation.z))*
			glm::scale(component.Scale),
			static_cast<uint32_t>(entity.GetComponent<Skateboard::StaticMeshInstanceComponent>().MeshId),
			entity.GetComponent<Skateboard::StaticMeshInstanceComponent>().MaterialId
		};
		m_ActiveScene->UploadInstanceData(entity, &buff, &buff.WorldMatrix);
	}

	DrawComponent<Skateboard::CameraComponent>("Camera", entity, [](auto& component) -> bool
		{
			Skateboard::PerspectiveCamera& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStr[] =
			{
				"Perspective",
				"Orthographic"
			};

			//const char* currentProjectionType = projectionTypeStr[(uint32_t)component.Camera.GetProjectionType()];

			//if (ImGui::BeginCombo("Projection", currentProjectionType))
			//{
			//	for (int i = 0; i < 2; ++i)
			//	{
			//		bool isSelected = currentProjectionType == projectionTypeStr[i];
			//		if (ImGui::Selectable(projectionTypeStr[i], isSelected))
			//		{
			//			currentProjectionType = projectionTypeStr[i];
			//			//TODO: Add functionality to switch between perspective and orthographic camera here!
			//			//camera.Set((MainCamera::ProjectionType)i);
			//		}

			//		if (isSelected)
			//		{
			//			ImGui::SetItemDefaultFocus();
			//		}

			//	}
			//	ImGui::EndCombo();
			//}

			/* begin if
			if (component.Camera.GetProjectionType() == (INT)MainCamera::ProjectionType::Perspective)
			{
			*/
				float perspectiveFov = glm::degrees(camera.GetFov());
				if (ImGui::DragFloat("Fov", &perspectiveFov))
				{
					camera.SetFov(glm::radians(perspectiveFov));
				}
				float perspectiveNear = camera.GetNearPlane();
				if (ImGui::DragFloat("Near Clip", &perspectiveNear))
				{
					camera.SetNearPlane(perspectiveNear);
				}
				float perspectiveFar = camera.GetFarPlane();
				if (ImGui::DragFloat("Far Clip", &perspectiveFar))
				{
					camera.SetFarPlane(perspectiveFar);
				}
				/*
					} ^ end if ^
				 */

		//TODO: Might be a *nice* feature, to support both perspective and orthographic cameras.

			/*if (component.Camera.GetProjectionType() == (INT)MainCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
				{
					camera.SetOrthographicSize(orthoSize);
				}
				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near Clip", &orthoNear))
				{
					camera.SetOrthographicNearClip(orthoNear);
				}
				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far Clip", &orthoFar))
				{
					camera.SetOrthographicFarClip(orthoFar);
				}

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}*/
				return false;
		});

	if(DrawComponent<Skateboard::MaterialComponent>("Material Component", entity, [](auto& component) -> bool
		{
			bool modified = false;
			Skateboard::Material& mat = component.Material;

			float4	albedo		= mat.GetAlbedo();
			int32_t albedoMap	= mat.GetAlbedoMapIndex();
			float	roughness	= mat.GetRoughness();
			float3	fresnel		= mat.GetFresnel();
			float	metallic	= mat.GetMetallic();
			float3	specular	= mat.GetSpecular();

			modified |= ImGui::DragFloat4("Albedo", &albedo.x, .1f, 0.f, 100.f);
			//modified |= ImGui::DragInt("AlbedoMap", &albedoMap, .1f, -1, Skateboard::AssetManager::GetTexureMaxIndex());
			modified |= ImGui::DragFloat("Roughness", &roughness, .25f, 0.f, 1.f);
			modified |= ImGui::DragFloat3("Fresnel", &fresnel.x, .25f, 0.f, 0.9f);
			modified |= ImGui::DragFloat("Metallic", &metallic, .1f, 0.f, 1.f);
			modified |= ImGui::DragFloat3("Specular", &specular.x, .1f, 0.f, 1.f);

			if (modified)
			{
				mat.SetAlbedo(albedo);
				mat.SetAlbedoMapIndex(albedoMap);
				mat.SetRoughness(roughness);
				mat.SetFresnel(fresnel);
				mat.SetMetallic(metallic);
				mat.SetSpecular(specular);
			}
			return modified;
		}))
	{
	}

	if (DrawComponent<Skateboard::StaticMeshInstanceComponent>("Static Mesh Instance Component", entity, [this](auto& component) -> bool
		{
			bool modified = false;
			const std::string& entityMesh = m_ActiveScene->GetMeshTag(component.MeshId);

			if (ImGui::BeginCombo("Mesh", entityMesh.c_str()))
			{
				for (uint32_t i = 0; i < m_ActiveScene->GetTotalMeshCount(); ++i)
				{
					Skateboard::MeshID currentMeshID = static_cast<Skateboard::MeshID>(i);
					const std::string& currentMesh = m_ActiveScene->GetMeshTag(currentMeshID);

					bool isSelected = !currentMesh.compare(entityMesh);
					if (ImGui::Selectable(currentMesh.c_str(), isSelected))
					{
						m_ActiveScene->ChangeInstanceMeshID(component, currentMeshID);
						modified = true;
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}

				}
				ImGui::EndCombo();
			}
			ImGui::Text("Instance ID: %u", component.InstanceId);

			// TODO: Remove this and handle internally based on the material component added
			modified |= ImGui::DragInt("Material Index", &component.MaterialId, .1f, -1, Skateboard::AssetManager::GetTexureMaxIndex());

			return modified;
		}))
	{
		// If an entity mesh instance component has been changed, then we need to rebuild the instance buffer
		m_ActiveScene->ResetInstanceBufferForAllEntities();
	}
}