#pragma once

#include <string>

class Element {
private:
	Element(const std::string& name);
public:
	std::string Name;

	// Initialise in .cpp
	static const Element FIRE;
	static const Element WATER;
	static const Element WIND;
	static const Element DARK;

	static const Element Next(const Element e);

	bool operator==(const Element& other) const;
};