#include "Element.h"
#include <cassert>

Element::Element(const std::string& name) : Name{ name } {}

const Element Element::FIRE("Fire");
const Element Element::WATER("Water");
const Element Element::WIND("Wind");
const Element Element::DARK("Dark");

bool Element::operator==(const Element& other) const {
	return Name == other.Name;
}

const Element Element::Next(const Element current) {
	if (current == FIRE) return WATER;
	else if (current == WATER) return WIND;
	else if (current == WIND) return DARK;
	else if (current == DARK) return FIRE;
	else assert(false);
}