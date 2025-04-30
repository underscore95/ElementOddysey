#pragma once

#include "Skateboard/Scene/Scene.h"
#include "Skateboard/Scene/Entity.h"
#include "Skateboard/Scene/Components.h"


class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const std::shared_ptr<Skateboard::Scene>& context);

	// @brief Set the current scene's context.
	// @param[in] Takes a reference to a shared pointer to a scene object.
	void SetContext(const std::shared_ptr<Skateboard::Scene>& context);

	// @brief Renders all the ImGui panels.
	void OnImGuiRender();

public:
	Skateboard::Entity GetSelectedEntity() const { return m_SelectionContext; }

private:

	// @brief Renders entity node in the scene's hierarchy panel.
	// @param[in] Takes an Entity as the argument.
	void DrawEntityNode(Skateboard::Entity entity);

	// @brief Renders all the components currently tied to the entity.
	// @param[in] Takes an Entity as the argument.
	void DrawComponents(Skateboard::Entity selected_context);

	template <typename T, typename UIFunction> bool DrawComponent(const std::string& name, Skateboard::Entity entity, UIFunction uiFunction);

private:
	std::shared_ptr<Skateboard::Scene> m_ActiveScene;
	Skateboard::Entity m_SelectionContext;
};
