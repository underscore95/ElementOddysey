#pragma once

/*
* \return The first entity with the component, or nullptr if neither have the component
*/
template <typename T>
inline Skateboard::Entity* getWithComponent(Skateboard::Entity* ent1, Skateboard::Entity* ent2) {
	if (ent1->HasComponent<T>()) return ent1;
	else if (ent2->HasComponent<T>()) return ent2;
	else return nullptr;
}