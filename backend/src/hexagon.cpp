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