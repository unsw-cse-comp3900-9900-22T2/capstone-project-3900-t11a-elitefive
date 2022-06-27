#include "hexagon.hpp"

// Hexagon Constructors
Hexagon::Hexagon(axial::vector pos) 
: Hexagon::Hexagon(pos, -1, -1) {}
Hexagon::Hexagon(axial::vector pos, int index) 
: Hexagon::Hexagon(pos, index, -1) {}

Hexagon::Hexagon(axial::vector pos, int index, int used) 
: position_{pos}
, lookup_index_{index}
, used_{used} 
{}

auto Hexagon::valid() const -> bool {
	if (used_ == Hexagon::EMPTY) {
		return false;
	}
	return true;
}

auto Hexagon::setTile(int player) -> void {
	used_ = player;
}

